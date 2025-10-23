#include <cu0/proc/process.hh>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported =>
//!         a compile-time warning will be present
//!     else (if all features are supported) =>
//!         no feature-related compile-time warnings will be present
#if !__has_include(<sys/types.h>) || !__has_include(<sys/wait.h>)
#warning <sys/types.h> or <sys/wait.h> is not found => \
cu0::Process::terminationCode() will not be used in the example
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
  someProcess.wait();
  //! @note not supported on all platforms yet
  //! @note exit code can be obtained only after a call to the wait function
  //! @note exit code contains exit status code of the created process
  const auto& terminationCode = someProcess.terminationCode();
  if (!terminationCode.has_value()) {
    std::cout << "The termination signal was not sent" << '\n';
  } else {
    std::cout << "Termination code of the created process: " <<
        *terminationCode << '\n';
  }
}

#endif
