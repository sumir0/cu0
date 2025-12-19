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
cu0::Strand::detached(const bool) will not be used in the example
int main() {}
#else

int main() {
  auto variant = cu0::Strand::create([](){ return; });
  if (!std::holds_alternative<cu0::Strand>(variant)) {
    std::cerr << "Error: the strand couldn't be created" << '\n';
    return 1;
  }
  auto& strand = std::get<cu0::Strand>(variant);
  const auto setDetachedVariant =
      strand.detached<cu0::Strand::Stage::NOT_LAUNCHED>(true);
  if (!std::holds_alternative<std::monostate>(setDetachedVariant)) {
    std::cout <<
        "Error: the detached state of the strand couldn't be modified" << '\n';
    return 2;
  }
  const auto runResult = strand.run();
  if (!std::holds_alternative<std::monostate>(runResult)) {
    std::cout << "Error: the strand couldn't be launched" << '\n';
    return 3;
  }
  //! no join needed
}

#endif
