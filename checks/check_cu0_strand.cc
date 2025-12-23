#include <cu0/proc/strand.hh>
#include <atomic> //! for storing task execution result
#include <cassert>
#include <thread> //! for a sleep inside a strand
#include <chrono> //! for specifying a sleep duration
#if __has_include(<pthread.h>)
  #include <pthread.h> //! for validating priority, policy, and stack size
#endif
#if __has_include(<unistd.h>)
  #include <unistd.h> //! for validating stack size
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
    constexpr auto variant_of_enums = std::variant<EnumA, EnumB>{EnumB::B};
    static_assert(!std::holds_alternative<EnumA>(variant_of_enums));
    static_assert(std::holds_alternative<EnumB>(variant_of_enums));
    static_assert(std::get<EnumB>(variant_of_enums) == EnumB::B);
  }

  //! check create function
  {
    auto atomic = std::atomic<bool>{false};
    const auto task = [&atomic]() { atomic = true; };
    auto strand_create_variant = cu0::Strand::create(task);
    assert(std::holds_alternative<cu0::Strand>(strand_create_variant));
    auto strand_task = std::get<cu0::Strand>(strand_create_variant);
    assert(atomic == false);
    const auto run_result = strand_task.run();
    assert(std::holds_alternative<std::monostate>(run_result));
    const auto join_result = strand_task.join();
    assert(std::holds_alternative<std::monostate>(join_result));
    assert(atomic == true);
  }

  auto stop = std::atomic<bool>(false);
  auto strand_create_variant = cu0::Strand::create([&stop]() {
    while (!stop) {
      std::this_thread::sleep_for(std::chrono::milliseconds{256});
    };
    return;
  });
  assert(std::holds_alternative<cu0::Strand>(strand_create_variant));

  auto& strand = std::get<cu0::Strand>(strand_create_variant);

#if __has_include(<pthread.h>)
  const auto min_priority_round_robin = sched_get_priority_min(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
  );

  {
    //! check scheduling getter
    const auto scheduling_get_variant =
        strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>();
    assert(std::holds_alternative<typename cu0::Strand::Scheduling>(
        scheduling_get_variant
    ));
    const auto& scheduling =
        std::get<typename cu0::Strand::Scheduling>(scheduling_get_variant);
    assert(scheduling.policy == cu0::Strand::Policy::PTHREAD_OTHER);
    assert(scheduling.priority == 0);

    //! check scheduling setter
    const auto policy_to_set = cu0::Strand::Policy::PTHREAD_RR;
    const auto priority_to_set = min_priority_round_robin;
    const auto scheduling_set_variant =
        strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
          .policy = policy_to_set,
          .priority = priority_to_set,
        });
    assert(std::holds_alternative<std::monostate>(scheduling_set_variant));
    {
      const auto scheduling_get_variant_after_set =
          strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<typename cu0::Strand::Scheduling>(
          scheduling_get_variant_after_set
      ));
      const auto& scheduling = std::get<typename cu0::Strand::Scheduling>(
          scheduling_get_variant_after_set
      );
      assert(scheduling.policy == policy_to_set);
      assert(scheduling.priority == priority_to_set);
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
    const auto priority_get_variant =
        strand.priority<cu0::Strand::Stage::NOT_LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::PriorityType
    >(priority_get_variant));
    const auto& priority = std::get<
        typename cu0::Strand::PriorityType
    >(priority_get_variant);
    assert(priority == min_priority_round_robin);

    auto priority_to_set = priority + 1;

    //! change the policy and the priority if needed
    const auto max_priority_round_robin = sched_get_priority_max(
        static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
    );
    if (priority_to_set > max_priority_round_robin) {
      const auto min_priority_other = sched_get_priority_min(
          static_cast<int>(cu0::Strand::Policy::PTHREAD_OTHER)
      );
      const auto policy_to_set = cu0::Strand::Policy::PTHREAD_OTHER;
      priority_to_set = min_priority_other;
      const auto scheduling_set_variant =
          strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
            .policy = policy_to_set,
            .priority = priority_to_set,
          });
      assert(std::holds_alternative<std::monostate>(scheduling_set_variant));
      {
        const auto scheduling_get_variant_after_set =
            strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>();
        assert(std::holds_alternative<typename cu0::Strand::Scheduling>(
            scheduling_get_variant_after_set
        ));
        const auto& scheduling = std::get<typename cu0::Strand::Scheduling>(
            scheduling_get_variant_after_set
        );
        assert(scheduling.policy == policy_to_set);
        assert(scheduling.priority == priority_to_set);
      }
    }

    //! check priority setter
    const auto priority_set_variant =
        strand.priority<cu0::Strand::Stage::NOT_LAUNCHED>(priority_to_set);
    assert(std::holds_alternative<std::monostate>(priority_set_variant));
    {
      const auto priority_get_variant_after_set =
          strand.priority<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<
          typename cu0::Strand::PriorityType
      >(priority_get_variant_after_set));
      const auto& priority = std::get<
          typename cu0::Strand::PriorityType
      >(priority_get_variant_after_set);
      assert(priority == priority_to_set);
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
  const auto policy_to_set = cu0::Strand::Policy::PTHREAD_OTHER;
  const auto min_priority_other = sched_get_priority_min(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_OTHER)
  );
  const auto priority_to_set = min_priority_other;
  const auto scheduling_set_variant =
      strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
        .policy = policy_to_set,
        .priority = priority_to_set,
      });
  assert(std::holds_alternative<std::monostate>(scheduling_set_variant));
  const auto scheduling_get_variant_after_set =
      strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>();
  assert(std::holds_alternative<typename cu0::Strand::Scheduling>(
      scheduling_get_variant_after_set
  ));
  const auto& scheduling = std::get<typename cu0::Strand::Scheduling>(
      scheduling_get_variant_after_set
  );
  assert(scheduling.policy == policy_to_set);
  assert(scheduling.priority == priority_to_set);
#endif

  //! check run function
  const auto run_variant = strand.run();
  assert(std::holds_alternative<std::monostate>(run_variant));

#if __has_include(<pthread.h>)
  {
    //! check scheduling getter after launch
    const auto scheduling_get_variant =
        strand.scheduling<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::Scheduling
    >(scheduling_get_variant));
    const auto& scheduling =
        std::get<typename cu0::Strand::Scheduling>(scheduling_get_variant);
    assert(scheduling.policy == policy_to_set);
    assert(scheduling.priority == priority_to_set);
  }
  {
    //! check scheduling setter after launch
    const auto scheduling_set_variant =
        strand.scheduling<cu0::Strand::Stage::LAUNCHED>(cu0::Strand::Scheduling{
          //! permissions may not be enough for other policies
          .policy = cu0::Strand::Policy::PTHREAD_OTHER,
          //! permissions may not be enough for other priority values
          .priority = min_priority_other,
        });
    assert(std::holds_alternative<std::monostate>(scheduling_set_variant));
  }
  {
    //! check scheduling getter after setter
    const auto scheduling_get_variant =
        strand.scheduling<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::Scheduling
    >(scheduling_get_variant));
    const auto& scheduling =
        std::get<typename cu0::Strand::Scheduling>(scheduling_get_variant);
    assert(scheduling.policy == cu0::Strand::Policy::PTHREAD_OTHER);
    assert(scheduling.priority == min_priority_other);
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
    const auto priority_get_variant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::PriorityType
    >(priority_get_variant));
    const auto& priority =
        std::get<typename cu0::Strand::PriorityType>(priority_get_variant);
    assert(priority == min_priority_other);
  }
  {
    //! check priority setter after launch
    const auto priority_set_variant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>(min_priority_other);
    assert(std::holds_alternative<std::monostate>(priority_set_variant));
  }
  {
    //! check priority getter after setter
    const auto priority_get_variant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::PriorityType
    >(priority_get_variant));
    const auto& priority =
        std::get<typename cu0::Strand::PriorityType>(priority_get_variant);
    assert(priority == min_priority_other);
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
    auto strand_for_detach_create_variant =
      cu0::Strand::create([](){ return; });
    assert(std::holds_alternative<cu0::Strand>(
        strand_for_detach_create_variant
    ));

    auto& strand_for_detach =
        std::get<cu0::Strand>(strand_for_detach_create_variant);

    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(!detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(!detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(false);
      assert(std::holds_alternative<std::monostate>(detached_variant));
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(!detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(!detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(true);
      assert(std::holds_alternative<std::monostate>(detached_variant));
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(false);
      assert(std::holds_alternative<std::monostate>(detached_variant));
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(!detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(!detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(false);
      assert(std::holds_alternative<std::monostate>(detached_variant));
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(!detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(!detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(true);
      assert(std::holds_alternative<std::monostate>(detached_variant));
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>(true);
      assert(std::holds_alternative<std::monostate>(detached_variant));
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
      assert(detached);
    }
    {
      const auto detached_variant =
          strand_for_detach.detached<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<bool>(detached_variant));
      const auto& detached = std::get<bool>(detached_variant);
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
    auto strand_for_stack_size_create_variant =
        cu0::Strand::create([](){ return; });
    assert(std::holds_alternative<cu0::Strand>(
        strand_for_stack_size_create_variant
    ));

    auto& strand_for_stack_size =
        std::get<cu0::Strand>(strand_for_stack_size_create_variant);

    {
      const auto get_stack_size_variant =
          strand_for_stack_size.stack_size<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<std::size_t>(get_stack_size_variant));
      //! default stack size is implementation defined
    }
#if __has_include(<stdlib.h>) && __has_include(<unistd.h>)
      const auto page_size = sysconf(_SC_PAGE_SIZE);
      assert(page_size > 0);
      const auto min_stack_size =
          (PTHREAD_STACK_MIN / page_size + 1) * page_size;
    {
      const auto allocate_stack_variant =
          strand_for_stack_size.allocate_stack<
              cu0::Strand::Stage::NOT_LAUNCHED
          >(min_stack_size);
      assert(std::holds_alternative<std::monostate>(allocate_stack_variant));
    }
    {
      const auto get_stack_size_variant =
          strand_for_stack_size.stack_size<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<std::size_t>(get_stack_size_variant));
      const auto stack_size = std::get<std::size_t>(get_stack_size_variant);
      assert(stack_size == min_stack_size);
    }
    {
      const auto get_stack_size_variant =
          strand_for_stack_size.stack_size<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<std::size_t>(get_stack_size_variant));
      const auto stack_size = std::get<std::size_t>(get_stack_size_variant);
      assert(stack_size == min_stack_size);
    }
    {
      const auto deallocate_stack_variant =
          strand_for_stack_size.deallocate_stack<
              cu0::Strand::Stage::TERMINATED
          >();
      assert(std::holds_alternative<std::monostate>(deallocate_stack_variant));
    }
    {
      const auto allocate_stack_variant =
          strand_for_stack_size.allocate_stack<
              cu0::Strand::Stage::NOT_LAUNCHED
          >(4 * min_stack_size);
      assert(std::holds_alternative<std::monostate>(allocate_stack_variant));
    }
    {
      const auto get_stack_size_variant =
          strand_for_stack_size.stack_size<cu0::Strand::Stage::NOT_LAUNCHED>();
      assert(std::holds_alternative<std::size_t>(get_stack_size_variant));
      const auto stack_size = std::get<std::size_t>(get_stack_size_variant);
      assert(stack_size == 4 * min_stack_size);
    }
    const auto run_variant = strand_for_stack_size.run();
    assert(std::holds_alternative<std::monostate>(run_variant));
    const auto join_variant = strand_for_stack_size.join();
    assert(std::holds_alternative<std::monostate>(join_variant));
    {
      const auto deallocate_stack_variant =
          strand_for_stack_size.deallocate_stack<
              cu0::Strand::Stage::TERMINATED
          >();
      assert(std::holds_alternative<std::monostate>(deallocate_stack_variant));
    }
  }
#else
#warning <stdlib.h> or <unistd.h> is not found => \
cu0::Strand::allocate_stack<cu0::Strand::Stage::NOT_LAUNCHED>(\
const std::size_t) will not be checked
#warning <stdlib.h> or <unistd.h> is not found => \
cu0::Strand::deallocate_stack<cu0::Strand::Stage::TERMINATED>() \
will not be checked
#endif
#else
#warning <pthread.h> is not found => \
cu0::Strand::stack_size<cu0::Strand::Stage::NOT_LAUNCHED>() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::allocate_stack<cu0::Strand::Stage::NOT_LAUNCHED>(\
const std::size_t) will not be checked
#warning <stdlib.h> or <unistd.h> is not found => \
cu0::Strand::deallocate_stack<cu0::Strand::Stage::TERMINATED>() \
will not be checked
#endif

  {
    auto strand_for_detach_create_variant =
        cu0::Strand::create([](){ return; });
    assert(std::holds_alternative<cu0::Strand>(
        strand_for_detach_create_variant
    ));

    auto& strand_for_detach =
        std::get<cu0::Strand>(strand_for_detach_create_variant);

    //! detach before launch will result in unspecified behaviour
    {
      const auto run_variant = strand_for_detach.run();
      assert(std::holds_alternative<std::monostate>(run_variant));
    }

    {
      const auto detach_variant =
          strand_for_detach.detach();
      assert(std::holds_alternative<std::monostate>(detach_variant));
    }
    //! repeat of detach will result in unspecified behaviour
  }

  //! check join function
  stop = true;
  const auto join_variant = strand.join();
  assert(std::holds_alternative<std::monostate>(join_variant));

  return 0;
}
