#include <cu0/proc/process.hh>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported =>
//!         a compile-time warning will be present
//!     else (if all features are supported) =>
//!         no feature-related compile-time warnings will be present
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
cu0::Process::stderr_cautious() will not be used in the example
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
  //! @note stderr returns standard error output of the created process
  //!     at the moment of call
  const auto [result, err_str] = some_process.stderr_cautious();
  if (!std::holds_alternative<std::monostate>(result)) {
    std::cout << "Error: data was not fully received from the stderr" << '\n';
  }
  if (err_str.empty()) {
    std::cout << "Stderr of the created process is empty" << '\n';
  } else {
    std::cout << "Stderr of the created process: " << err_str << '\n';
  }
}

#endif
