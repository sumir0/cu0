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
  //! @note and returns a variant
  //!     if no error was reported => returns std::monostate
  //!     else => error code of a reported error
  //! @note if the process is active => will block
  const auto result = someProcess.waitCautious();
  if (std::holds_alternative<std::monostate>(result)) {
    std::cout << "No error was reported" << '\n';
  } else {
    std::cout << "There was an error. Error code: " <<
        static_cast<int>(std::get<cu0::Process::WaitError>(result)) << '\n';
  }
}

#endif
