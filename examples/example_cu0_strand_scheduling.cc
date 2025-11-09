#include <cu0/proc/strand.hh>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported =>
//!         a compile-time warning will be present
//!     else (if all features are supported) =>
//!         no feature-related compile-time warnings will be present
#if !__has_include(<pthread.h>)
#warning <pthread.h> is not found => \
cu0::Strand::scheduling(const Scheduling&) will not be used in the example
#warning <pthread.h> is not found => \
cu0::Strand::scheduling() will not be used in the example
int main() {}
#else

int main() {
  auto variant = cu0::Strand::create([](){ return; });
  if (!std::holds_alternative<cu0::Strand>(variant)) {
    std::cerr << "Error: the strand couldn't be created" << '\n';
    return 1;
  }
  auto& strand = std::get<cu0::Strand>(variant);
  const auto policyToSet = cu0::Strand::Policy::PTHREAD_OTHER;
  const auto minPriority =
      sched_get_priority_min(static_cast<int>(policyToSet));
  const auto setSchedulingResult =
      strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
        .policy = policyToSet,
        .priority = minPriority,
      });
  if (!std::holds_alternative<std::monostate>(setSchedulingResult)) {
    std::cout << "Error: the scheduling of the strand couldn't be modified" <<
        '\n';
    return 2;
  }
  const auto runResult = strand.run();
  if (!std::holds_alternative<std::monostate>(runResult)) {
    std::cout << "Error: the strand couldn't be launched" << '\n';
    return 3;
  }
  const auto getSchedulingResult =
      strand.scheduling<cu0::Strand::Stage::LAUNCHED>();
  if (!std::holds_alternative<typename cu0::Strand::Scheduling>(
      getSchedulingResult
  )) {
    std::cout << "Error: the scheduling of the strand couldn't be retrieved" <<
        '\n';
  } else {
    const auto& scheduling =
        std::get<typename cu0::Strand::Scheduling>(getSchedulingResult);
    std::cout << "Info: the strand has scheduling == {" << '\n';
    std::string policyString;
    switch (static_cast<int>(scheduling.policy)) {
    case SCHED_OTHER:
      policyString = "SCHED_OTHER";
      break;
    case SCHED_FIFO:
      policyString = "SCHED_FIFO";
      break;
    case SCHED_RR:
      policyString = "SCHED_RR";
      break;
    default:
      policyString = "?";
      break;
    }
    std::cout << "  .policy = " << policyString << "," <<
        '\n';
    std::cout << "  .priority = " << scheduling.priority << "," << '\n';
    std::cout << "}" << '\n';
  }
  const auto joinResult = strand.join();
  if (!std::holds_alternative<std::monostate>(joinResult)) {
    std::cout << "Error: the strand couldn't be joined" << '\n';
  }
}

#endif
