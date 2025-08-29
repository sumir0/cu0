#include <cu0/proc/process.hh>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported ->
//!         a compile-time warning will be present
//!     else (if all features are supported) ->
//!         no feature-related compile-time warnings will be present
#ifndef __unix__
#warning __unix__ is not defined => \
    cu0::Process::current() will not be used in the example => \
    cu0::Process::pid() will not be used in the example
int main() {}
#else
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
    cu0::Process::current() will not be used in the example => \
    cu0::Process::pid() will not be used in the example
int main() {}
#else

int main() {
  const auto thisProcess = cu0::Process::current();
  //! @note pid contains value of the process identifier of the current process
  const auto& pid = thisProcess.pid();
  std::cout << "This process identifier: " << pid << '\n';
}

#endif
#endif
