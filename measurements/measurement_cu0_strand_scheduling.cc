//! priveleges need to be enough for realtime scheduling
//! therefore, this measurement, probably, needs to be run by a superuser

#include <cu0/proc/strand.hh>
#include <cassert>
#if __has_include(<pthread.h>)
  #include <pthread.h> //! for validating priority and policy
#endif

int main() {
  auto strand_create_variant = cu0::Strand::create([](){ return; });
  assert(std::holds_alternative<cu0::Strand>(strand_create_variant));

  auto& strand = std::get<cu0::Strand>(strand_create_variant);

#if __has_include(<pthread.h>)
  const auto min_priority_round_robin = sched_get_priority_min(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
  );
  const auto max_priority_round_robin = sched_get_priority_max(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
  );
  const auto policy_to_set = cu0::Strand::Policy::PTHREAD_RR;
  auto priorityToSet = min_priority_round_robin;

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
    const auto scheduling_set_variant =
        strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
          .policy = policy_to_set,
          .priority = priorityToSet,
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
    const auto priority_get_variant =
        strand.priority<cu0::Strand::Stage::NOT_LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::PriorityType
    >(priority_get_variant));
    const auto& priority = std::get<
        typename cu0::Strand::PriorityType
    >(priority_get_variant);
    assert(priority == min_priority_round_robin);

    if (min_priority_round_robin + 1 <= max_priority_round_robin) {
      priorityToSet = min_priority_round_robin + 1;
    }

    //! check priority setter
    const auto priority_set_variant =
        strand.priority<cu0::Strand::Stage::NOT_LAUNCHED>(priorityToSet);
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

  //! check run function
  const auto runVariant = strand.run();
  assert(std::holds_alternative<std::monostate>(runVariant));

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
    assert(scheduling.priority == priorityToSet);
  }

  if (min_priority_round_robin + 2 <= max_priority_round_robin) {
    priorityToSet = min_priority_round_robin + 2;
  }

  {
    //! check scheduling setter after launch
    const auto scheduling_set_variant =
        strand.scheduling<cu0::Strand::Stage::LAUNCHED>(cu0::Strand::Scheduling{
          .policy = policy_to_set,
          .priority = priorityToSet,
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
    assert(scheduling.policy == policy_to_set);
    assert(scheduling.priority == priorityToSet);
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
    assert(priority == priorityToSet);
  }

  if (min_priority_round_robin + 3 <= min_priority_round_robin) {
    priorityToSet = min_priority_round_robin + 3;
  }

  {
    //! check priority setter after launch
    const auto priority_set_variant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>(priorityToSet);
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
    assert(priority == priorityToSet);
  }
#else
#warning <pthread.h> is not found => \
cu0::Strand::priority<cu0::Strand::Stage::LAUNCHED>() will not be checked
#warning <pthread.h> is not found => \
cu0::Strand::priority<cu0::Strand::Stage::LAUNCHED>(const priority_type&) will \
not be checked
#endif

  //! check join function
  const auto join_variant = strand.join();
  assert(std::holds_alternative<std::monostate>(join_variant));

  return 0;
}
