#include <cu0/proc/strand.hh>
#include <atomic> //! for storing task execution result
#include <cassert>
#if __has_include(<pthread.h>)
  #include <pthread.h> //! for validating priority and policy
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
  const auto minPriorityRoundRobin = sched_get_priority_min(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
  );

  {
    //! check scheduling getter
    const auto schedulingGetVariant =
        strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>();
    assert(std::holds_alternative<typename cu0::Strand::Scheduling>(
        schedulingGetVariant
    ));
    const auto& scheduling =
        std::get<typename cu0::Strand::Scheduling>(schedulingGetVariant);
    assert(scheduling.policy == cu0::Strand::Policy::PTHREAD_OTHER);
    assert(scheduling.priority == 0);

    //! check scheduling setter
    const auto policyToSet = cu0::Strand::Policy::PTHREAD_RR;
    const auto priorityToSet = minPriorityRoundRobin;
    const auto schedulingSetVariant =
        strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
          .policy = policyToSet,
          .priority = priorityToSet,
        });
    assert(std::holds_alternative<std::monostate>(schedulingSetVariant));
    {
      const auto schedulingGetVariantAfterSet =
          strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<typename cu0::Strand::Scheduling>(
          schedulingGetVariantAfterSet
      ));
      const auto& scheduling = std::get<typename cu0::Strand::Scheduling>(
          schedulingGetVariantAfterSet
      );
      assert(scheduling.policy == policyToSet);
      assert(scheduling.priority == priorityToSet);
    }
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::scheduling<cu0::Strand::Stage::NOT_LAUNCHED>() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::scheduling<cu0::Strand::Stage::NOT_LAUNCHED>(const Scheduling&) \
will not be checked
#endif

#if __has_include(<pthread.h>)
  {
    //! check priority getter
    const auto priorityGetVariant =
        strand.priority<cu0::Strand::Stage::NOT_LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::priority_type
    >(priorityGetVariant));
    const auto& priority = std::get<
        typename cu0::Strand::priority_type
    >(priorityGetVariant);
    assert(priority == minPriorityRoundRobin);

    auto priorityToSet = priority + 1;

    //! change the policy and the priority if needed
    const auto maxPriorityRoundRobin = sched_get_priority_max(
        static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
    );
    if (priorityToSet > maxPriorityRoundRobin) {
      const auto minPriorityOther = sched_get_priority_min(
          static_cast<int>(cu0::Strand::Policy::PTHREAD_OTHER)
      );
      const auto policyToSet = cu0::Strand::Policy::PTHREAD_OTHER;
      priorityToSet = minPriorityOther;
      const auto schedulingSetVariant =
          strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
            .policy = policyToSet,
            .priority = priorityToSet,
          });
      assert(std::holds_alternative<std::monostate>(schedulingSetVariant));
      {
        const auto schedulingGetVariantAfterSet =
            strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>();
        assert(std::holds_alternative<typename cu0::Strand::Scheduling>(
            schedulingGetVariantAfterSet
        ));
        const auto& scheduling = std::get<typename cu0::Strand::Scheduling>(
            schedulingGetVariantAfterSet
        );
        assert(scheduling.policy == policyToSet);
        assert(scheduling.priority == priorityToSet);
      }
    }

    //! check priority setter
    const auto prioritySetVariant =
        strand.priority<cu0::Strand::Stage::NOT_LAUNCHED>(priorityToSet);
    assert(std::holds_alternative<std::monostate>(prioritySetVariant));
    {
      const auto priorityGetVariantAfterSet =
          strand.priority<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<
          typename cu0::Strand::priority_type
      >(priorityGetVariantAfterSet));
      const auto& priority = std::get<
          typename cu0::Strand::priority_type
      >(priorityGetVariantAfterSet);
      assert(priority == priorityToSet);
    }
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::priority<cu0::Strand::Stage::NOT_LAUNCHED>() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::priority<cu0::Strand::Stage::NOT_LAUNCHED>(const priority_type&) \
will not be checked
#endif

#if __has_include(<pthread.h>)
  //! change the policy to PTHREAD_OTHER and the priority to minimal allowed by
  //! the policy in order to prevent resource availability failure for other
  //! threads and processes on the machine
  const auto policyToSet = cu0::Strand::Policy::PTHREAD_OTHER;
  const auto minPriorityOther = sched_get_priority_min(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_OTHER)
  );
  const auto priorityToSet = minPriorityOther;
  const auto schedulingSetVariant =
      strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
        .policy = policyToSet,
        .priority = priorityToSet,
      });
  assert(std::holds_alternative<std::monostate>(schedulingSetVariant));
  const auto schedulingGetVariantAfterSet =
      strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>();
  assert(std::holds_alternative<typename cu0::Strand::Scheduling>(
      schedulingGetVariantAfterSet
  ));
  const auto& scheduling = std::get<typename cu0::Strand::Scheduling>(
      schedulingGetVariantAfterSet
  );
  assert(scheduling.policy == policyToSet);
  assert(scheduling.priority == priorityToSet);
#endif

  //! check run function
  const auto runVariant = strand.run();
  assert(std::holds_alternative<std::monostate>(runVariant));

#if __has_include(<pthread.h>)
  {
    //! check scheduling getter after launch
    const auto schedulingGetVariant =
        strand.scheduling<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::Scheduling
    >(schedulingGetVariant));
    const auto& scheduling =
        std::get<typename cu0::Strand::Scheduling>(schedulingGetVariant);
    assert(scheduling.policy == policyToSet);
    assert(scheduling.priority == priorityToSet);
  }
  {
    //! check scheduling setter after launch
    const auto schedulingSetVariant =
        strand.scheduling<cu0::Strand::Stage::LAUNCHED>(cu0::Strand::Scheduling{
          //! permissions may not be enough for other policies
          .policy = cu0::Strand::Policy::PTHREAD_OTHER,
          //! permissions may not be enough for other priority values
          .priority = minPriorityOther,
        });
    assert(std::holds_alternative<std::monostate>(schedulingSetVariant));
  }
  {
    //! check scheduling getter after setter
    const auto schedulingGetVariant =
        strand.scheduling<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::Scheduling
    >(schedulingGetVariant));
    const auto& scheduling =
        std::get<typename cu0::Strand::Scheduling>(schedulingGetVariant);
    assert(scheduling.policy == cu0::Strand::Policy::PTHREAD_OTHER);
    assert(scheduling.priority == minPriorityOther);
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::scheduling<cu0::Strand::Stage::LAUNCHED>() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::scheduling<cu0::Strand::Stage::LAUNCHED>(const Scheduling&) will \
not be checked
#endif

#if __has_include(<pthread.h>)
  {
    //! check priority getter after launch
    const auto priorityGetVariant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::priority_type
    >(priorityGetVariant));
    const auto& priority =
        std::get<typename cu0::Strand::priority_type>(priorityGetVariant);
    assert(priority == minPriorityOther);
  }
  {
    //! check priority setter after launch
    const auto prioritySetVariant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>(minPriorityOther);
    assert(std::holds_alternative<std::monostate>(prioritySetVariant));
  }
  {
    //! check priority getter after setter
    const auto priorityGetVariant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::priority_type
    >(priorityGetVariant));
    const auto& priority =
        std::get<typename cu0::Strand::priority_type>(priorityGetVariant);
    assert(priority == minPriorityOther);
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::priority<cu0::Strand::Stage::LAUNCHED>() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::priority<cu0::Strand::Stage::LAUNCHED>(const priority_type&) will \
not be checked
#endif

#if __has_include(<pthread.h>)
  {
    auto strandForDetachCreateVariant = cu0::Strand::create([](){ return; });
    assert(std::holds_alternative<cu0::Strand>(strandForDetachCreateVariant));

    auto& strandForDetach = std::get<cu0::Strand>(strandForDetachCreateVariant);

    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(!detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(!detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(false);
      assert(std::holds_alternative<std::monostate>(detachedVariant));
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(!detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(!detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(true);
      assert(std::holds_alternative<std::monostate>(detachedVariant));
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(false);
      assert(std::holds_alternative<std::monostate>(detachedVariant));
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(!detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(!detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(false);
      assert(std::holds_alternative<std::monostate>(detachedVariant));
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(!detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(!detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(true);
      assert(std::holds_alternative<std::monostate>(detachedVariant));
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(true);
      assert(std::holds_alternative<std::monostate>(detachedVariant));
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(detached);
    }
    {
      const auto detachedVariant =
          strandForDetach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detachedVariant));
      const auto& detached = std::get<bool>(detachedVariant);
      assert(detached);
    }
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::detached<cu0::Strand::Stage::NOT_LAUNCHED>() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::detached<cu0::Strand::Stage::NOT_LAUNCHED>(const bool) will not \
be checked
#endif

#if __has_include(<pthread.h>)
  {
    auto strandForDetachCreateVariant = cu0::Strand::create([](){ return; });
    assert(std::holds_alternative<cu0::Strand>(strandForDetachCreateVariant));

    auto& strandForDetach = std::get<cu0::Strand>(strandForDetachCreateVariant);

    //! detach before launch will result in unsoecified behaviour
    {
      const auto runVariant = strandForDetach.run();
      assert(std::holds_alternative<std::monostate>(runVariant));
    }

    {
      const auto detachVariant =
          strandForDetach.detach<cu0::Strand::Stage::LAUNCHED>();
      assert(std::holds_alternative<std::monostate>(detachVariant));
    }
    //! repeat of detach will result in unspecified behaviour
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::detach<cu0::Strand::Stage::LAUNCHED>() will not be checked
#endif

  //! check join function
  const auto joinVariant = strand.join();
  assert(std::holds_alternative<std::monostate>(joinVariant));

  return 0;
}
