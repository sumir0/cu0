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
    cu0::Process::create() will not be used in the example
int main() {}
#else
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
    cu0::Process::create() will not be used in the example
int main() {}
#else

int main() {
  //! @note not supported on all platforms yet
  //! @note executable can be run by a process
  const auto executable = cu0::Executable{ .binary = "a.out" };
  //! @note optional contains a process if it was created
  const std::variant<cu0::Process, cu0::Process::CreateError> variant =
      cu0::Process::create(executable);
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: No processes were created" << '\n';
  } else {
    //! @note createdProcess contains a representation of the running executable
    const auto& createdProcess = std::get<cu0::Process>(variant);
    std::cout << "Pid of the created process: " << createdProcess.pid() << '\n';
  }
}

#endif
#endif
