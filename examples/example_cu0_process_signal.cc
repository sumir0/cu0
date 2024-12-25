#include <cu0/proc.hxx>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported ->
//!         a compile-time warning will be present
//!     else (if all features are supported) ->
//!         no feature-related compile-time warnings will be present
#ifndef __unix__
#warning __unix__ is not defined => \
    cu0::Process::signal() will not be used in the example
int main() {}
#else
#if !__has_include(<signal.h>)
#warning <signal.h> is not found => \
    cu0::Process::signal() will not be used in the example
int main() {}
#else

int main() {
  const auto variant = cu0::Process::create(cu0::Executable{
    .binary = "someExecutable"
  });
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: the process was not created" << '\n';
  }
  const auto& someProcess = std::get<cu0::Process>(variant);
  //! @note not supported on all platforms yet
  //! @note signals the SIGTERM signal to the process
  const auto errorCode = someProcess.signal(SIGTERM);
  if (errorCode != cu0::Process::SignalError::NO_ERROR) {
    std::cout << "Error: the signal was not sent" << '\n';
  }
}

#endif
#endif
