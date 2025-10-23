#include <cu0/proc/strand.hh>
#include <atomic> //! for storing task execution result
#include <cassert>
#if __has_include(<pthread.h>)
  #include <pthread.h> //! for validating a priority with a policy
#endif

int main() {
  //! check that the compiler makes sense
  {
    enum struct EnumA {
      A = 1,
    };
    enum struct EnumB {
      B = 2,
    };
    constexpr auto variantOfEnums = std::variant<EnumA, EnumB>{EnumB::B};
    static_assert(!std::holds_alternative<EnumA>(variantOfEnums));
    static_assert(std::holds_alternative<EnumB>(variantOfEnums));
    static_assert(std::get<EnumB>(variantOfEnums) == EnumB::B);
  }

  //! check create function
  {
    auto atomic = std::atomic<bool>{false};
    const auto task = [&atomic]() { atomic = true; };
    auto strandCreateVariant = cu0::Strand::create(task);
    assert(std::holds_alternative<cu0::Strand>(strandCreateVariant));
    auto strandTask = std::get<cu0::Strand>(strandCreateVariant);
    assert(atomic == false);
    const auto runResult = strandTask.run();
    assert(std::holds_alternative<std::monostate>(runResult));
    const auto joinResult = strandTask.join();
    assert(std::holds_alternative<std::monostate>(joinResult));
    assert(atomic == true);
  }

  auto strandCreateVariant = cu0::Strand::create([](){ return; });
  assert(std::holds_alternative<cu0::Strand>(strandCreateVariant));

  auto& strand = std::get<cu0::Strand>(strandCreateVariant);

#if __has_include(<pthread.h>)
  const auto minPriority = sched_get_priority_min(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
  );

  {
    //! check policy getter
    const auto policyVariant = strand.policy();
    assert(std::holds_alternative<typename cu0::Strand::Policy>(
        policyVariant
    ));
    const auto& policy =
        std::get<typename cu0::Strand::Policy>(policyVariant);
    assert(policy == cu0::Strand::Policy::PTHREAD_OTHER);

    //! check policy setter
    const auto policyToSet = cu0::Strand::Policy::PTHREAD_RR;
    const auto policySetVariant = strand.policy(policyToSet);
    assert(std::holds_alternative<std::monostate>(policySetVariant));
    {
      const auto policyGetVariantAfterSet = strand.policy();
      assert(std::holds_alternative<typename cu0::Strand::Policy>(
          policyGetVariantAfterSet
      ));
      const auto& policy = std::get<typename cu0::Strand::Policy>(
          policyGetVariantAfterSet
      );
      assert(policy == policyToSet);
    }

    //! change the priority to minimal allowed by the policy
    const auto priorityToSet = minPriority;
    const auto prioritySetVariant = strand.priority(priorityToSet);
    assert(std::holds_alternative<std::monostate>(prioritySetVariant));
    const auto priorityGetVariantAfterSet = strand.priority();
    assert(std::holds_alternative<typename cu0::Strand::priority_type>(
        priorityGetVariantAfterSet
    ));
    const auto& priority = std::get<typename cu0::Strand::priority_type>(
        priorityGetVariantAfterSet
    );
    assert(priority == priorityToSet);
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::policy() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::policy(const Policy&) will not be checked
#endif

#if __has_include(<pthread.h>)
  {
    //! check priority getter
    const auto priorityGetVariant = strand.priority();
    assert(std::holds_alternative<typename cu0::Strand::priority_type>(
        priorityGetVariant
    ));
    const auto& priority =
        std::get<typename cu0::Strand::priority_type>(priorityGetVariant);
    assert(priority == minPriority);

    auto priorityToSet = priority + 1;

    //! change the policy and the priority if needed
    const auto maxPriority = sched_get_priority_max(
        static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
    );
    if (priorityToSet > maxPriority) {
      const auto minPriorityOther = sched_get_priority_min(
          static_cast<int>(cu0::Strand::Policy::PTHREAD_OTHER)
      );
      priorityToSet = minPriorityOther;
      const auto policyToSet = cu0::Strand::Policy::PTHREAD_OTHER;
      const auto policySetVariant = strand.policy(policyToSet);
      assert(std::holds_alternative<std::monostate>(policySetVariant));
      {
        const auto policyGetVariantAfterSet = strand.policy();
        assert(std::holds_alternative<typename cu0::Strand::Policy>(
            policyGetVariantAfterSet
        ));
        const auto& policy = std::get<typename cu0::Strand::Policy>(
            policyGetVariantAfterSet
        );
        assert(policy == policyToSet);
      }
    }

    //! check priority setter
    const auto prioritySetVariant = strand.priority(priorityToSet);
    assert(std::holds_alternative<std::monostate>(prioritySetVariant));
    {
      const auto priorityGetVariantAfterSet = strand.priority();
      assert(std::holds_alternative<typename cu0::Strand::priority_type>(
          priorityGetVariantAfterSet
      ));
      const auto& priority = std::get<typename cu0::Strand::priority_type>(
          priorityGetVariantAfterSet
      );
      assert(priority == priorityToSet);
    }
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::priority() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::priority(const priority_type&) will not be checked
#endif

#if __has_include(<pthread.h>)
  //! change the policy to PTHREAD_OTHER and the priority to minimal allowed by
  //! the policy in order to prevent resource availability failure for other
  //! threads and processes on the machine
  const auto policyToSet = cu0::Strand::Policy::PTHREAD_OTHER;
  const auto policySetVariant = strand.policy(policyToSet);
  assert(std::holds_alternative<std::monostate>(policySetVariant));
  const auto policyGetVariantAfterSet = strand.policy();
  assert(std::holds_alternative<typename cu0::Strand::Policy>(
      policyGetVariantAfterSet
  ));
  const auto& policy = std::get<typename cu0::Strand::Policy>(
      policyGetVariantAfterSet
  );
  assert(policy == policyToSet);
  const auto minPriorityOther = sched_get_priority_min(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_OTHER)
  );
  const auto priorityToSet = minPriorityOther;
  const auto prioritySetVariant = strand.priority(priorityToSet);
  assert(std::holds_alternative<std::monostate>(prioritySetVariant));
  const auto priorityGetVariantAfterSet = strand.priority();
  assert(std::holds_alternative<typename cu0::Strand::priority_type>(
      priorityGetVariantAfterSet
  ));
  const auto& priority = std::get<typename cu0::Strand::priority_type>(
      priorityGetVariantAfterSet
  );
  assert(priority == priorityToSet);
#endif

  //! check run function
  const auto runVariant = strand.run();
  assert(std::holds_alternative<std::monostate>(runVariant));

  //! check join function
  const auto joinVariant = strand.join();
  assert(std::holds_alternative<std::monostate>(joinVariant));

  return 0;
}
