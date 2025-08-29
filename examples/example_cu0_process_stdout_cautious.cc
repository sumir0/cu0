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
    cu0::Process::stdoutCautious() will not be used in the example
int main() {}
#else
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
    cu0::Process::stdoutCautious() will not be used in the example
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
  //! @note stdout returns standard output of the created process
  //!     at the moment of call
  const auto [outStr, errorCode] = someProcess.stdoutCautious();
  if (errorCode != cu0::Process::ReadError::NO_ERROR) {
    std::cout << "Error: data was not fully received from the stdout" << '\n';
  }
  if (outStr.empty()) {
    std::cout << "Stdout of the created process is empty" << '\n';
  } else {
    std::cout << "Stdout of the created process: " << outStr << '\n';
  }
}

#endif
#endif
