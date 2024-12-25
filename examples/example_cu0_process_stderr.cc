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
    cu0::Process::stderr() will not be used in the example
int main() {}
#else
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
    cu0::Process::stderr() will not be used in the example
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
  //! @note stderr contains standard output of the created process
  //!     at the moment of call
  const auto [errStr, errorCode] = someProcess.stderr();
  if (errorCode != cu0::Process::ReadError::NO_ERROR) {
    std::cout << "Error: data was not fully received from the stderr" << '\n';
  }
  if (errStr.empty()) {
    std::cout << "Stderr of the created process is empty" << '\n';
  } else {
    std::cout << "Stderr of the created process: " << errStr << '\n';
  }
}

#endif
#endif
