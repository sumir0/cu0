#include <cu0/proc/process.hh>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported =>
//!         a compile-time warning will be present
//!     else (if all features are supported) =>
//!         no feature-related compile-time warnings will be present
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
cu0::Process::current() will not be used in the example
int main() {}
#else

int main() {
  //! @note not supported on all platforms yet
  //! @note contains a representation of the current process
  const auto this_process = cu0::Process::current();
  std::cout << "Pid of this process: " << this_process.pid() << '\n';
}

#endif
