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
    cu0::Process::waitCautious() will not be used in the example
int main() {}
#else
#if !__has_include(<sys/types.h>) || !__has_include(<sys/wait.h>)
#warning <sys/types.h> or <sys/wait.h> is not found => \
    cu0::Process::waitCautious() will not be used in this example
int main() {}
#else

int main() {
  auto variant = cu0::Process::create(cu0::Executable{
    .binary = "someExecutable"
  });
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: the process was not created" << '\n';
  }
  auto& someProcess = std::get<cu0::Process>(variant);
  //! @note not supported on all platforms yet
  //! @note the waitCautious function waits for a process to execute
  //! @note and returns an error code
  //!     if no errors occured -> returns cu0::Process::WaitError::NO_ERROR
  //!     else -> an error has occured
  //! @note if the process is active -> will block
  const auto error = someProcess.waitCautious();
  switch (error) {
  case cu0::Process::WaitError::NO_ERROR:
    std::cout << "There were no errors" << '\n';
    break;
  default:
    std::cout << "There was an error. Error code: " <<
        static_cast<int>(error) << '\n';
    break;
  }
}

#endif
#endif
