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
cu0::Process::wait() will not be used in this example
int main() {}
#else

int main() {
  auto variant = cu0::Process::create(cu0::Executable{
    .binary = "some_executable"
  });
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: the process was not created" << '\n';
  }
  auto& some_process = std::get<cu0::Process>(variant);
  //! @note not supported on all platforms yet
  //! @note the wait function waits for a process to execute
  //! @note if the process is active => will block
  some_process.wait();
  const auto& exit_code = some_process.exit_code();
  if (!exit_code.has_value()) {
    std::cout << "Error: the exit code was not obtained" << '\n';
  } else {
    std::cout << "Exit code of the created process: " << *exit_code << '\n';
  }
}

#endif
