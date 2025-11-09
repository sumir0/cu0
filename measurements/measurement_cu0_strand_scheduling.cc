//! priveleges need to be enough for realtime scheduling
//! therefore, this measurement, probably, needs to be run by a superuser

#include <cu0/proc/strand.hh>
#include <cassert>
#if __has_include(<pthread.h>)
  #include <pthread.h> //! for validating priority and policy
#endif

int main() {
  auto strandCreateVariant = cu0::Strand::create([](){ return; });
  assert(std::holds_alternative<cu0::Strand>(strandCreateVariant));

  auto& strand = std::get<cu0::Strand>(strandCreateVariant);

#if __has_include(<pthread.h>)
  const auto minPriorityRoundRobin = sched_get_priority_min(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
  );
  const auto maxPriorityRoundRobin = sched_get_priority_max(
      static_cast<int>(cu0::Strand::Policy::PTHREAD_RR)
  );
  const auto policyToSet = cu0::Strand::Policy::PTHREAD_RR;
  auto priorityToSet = minPriorityRoundRobin;

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

    if (minPriorityRoundRobin + 1 <= maxPriorityRoundRobin) {
      priorityToSet = minPriorityRoundRobin + 1;
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

  if (minPriorityRoundRobin + 2 <= maxPriorityRoundRobin) {
    priorityToSet = minPriorityRoundRobin + 2;
  }

  {
    //! check scheduling setter after launch
    const auto schedulingSetVariant =
        strand.scheduling<cu0::Strand::Stage::LAUNCHED>(cu0::Strand::Scheduling{
          .policy = policyToSet,
          .priority = priorityToSet,
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
    assert(scheduling.policy == policyToSet);
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
    const auto priorityGetVariant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>();
    assert(std::holds_alternative<
        typename cu0::Strand::priority_type
    >(priorityGetVariant));
    const auto& priority =
        std::get<typename cu0::Strand::priority_type>(priorityGetVariant);
    assert(priority == priorityToSet);
  }

  if (minPriorityRoundRobin + 3 <= minPriorityRoundRobin) {
    priorityToSet = minPriorityRoundRobin + 3;
  }

  {
    //! check priority setter after launch
    const auto prioritySetVariant =
        strand.priority<cu0::Strand::Stage::LAUNCHED>(priorityToSet);
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
  const auto joinVariant = strand.join();
  assert(std::holds_alternative<std::monostate>(joinVariant));

  return 0;
}
