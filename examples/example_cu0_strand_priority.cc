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
cu0::Strand::priority(const priority_type&) will not be used in the example
int main() {}
#else

int main() {
  auto variant = cu0::Strand::create([](){ return; });
  if (!std::holds_alternative<cu0::Strand>(variant)) {
    std::cerr << "Error: the strand couldn't be created" << '\n';
  }
  auto& strand = std::get<cu0::Strand>(variant);
  const auto policyToSet = cu0::Strand::Policy::PTHREAD_OTHER;
  const auto setPolicyResult = strand.policy(policyToSet);
  if (!std::holds_alternative<std::monostate>(setPolicyResult)) {
    std::cout << "Error: the policy of the strand couldn't be set " << '\n';
  }
  const auto minPriority =
      sched_get_priority_min(static_cast<int>(policyToSet));
  const auto setPriorityResult = strand.priority(minPriority);
  if (!std::holds_alternative<std::monostate>(setPriorityResult)) {
    std::cout << "Error: the priority of the strand couldn't be set" << '\n';
  }
  const auto runResult = strand.run();
  if (!std::holds_alternative<std::monostate>(runResult)) {
    std::cout << "Error: the strand couldn't be launched" << '\n';
  }
  const auto joinResult = strand.join();
  if (!std::holds_alternative<std::monostate>(joinResult)) {
    std::cout << "Error: the strand couldn't be joined" << '\n';
  }
}

#endif
