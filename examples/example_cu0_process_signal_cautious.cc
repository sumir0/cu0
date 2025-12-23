#include <cu0/proc/process.hh>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported =>
//!         a compile-time warning will be present
//!     else (if all features are supported) =>
//!         no feature-related compile-time warnings will be present
#if !__has_include(<signal.h>)
#warning <signal.h> is not found => \
cu0::Process::signal_cautious() will not be used in the example
int main() {}
#else

int main() {
  const auto variant = cu0::Process::create(cu0::Executable{
    .binary = "some_executable"
  });
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: the process was not created" << '\n';
  }
  const auto& some_process = std::get<cu0::Process>(variant);
  //! @note not supported on all platforms yet
  //! @note signals the SIGTERM signal to the process
  const auto signal_result = some_process.signal_cautious(SIGTERM);
  if (!std::holds_alternative<std::monostate>(signal_result)) {
    std::cout << "Error: the signal was not sent" << '\n';
  }
}

#endif
