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
cu0::Process::stdout_cautious() will not be used in the example
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
  //! @note stdout returns standard output of the created process
  //!     at the moment of call
  const auto [result, out_str] = some_process.stdout_cautious();
  if (!std::holds_alternative<std::monostate>(result)) {
    std::cout << "Error: data was not fully received from the stdout" << '\n';
  }
  if (out_str.empty()) {
    std::cout << "Stdout of the created process is empty" << '\n';
  } else {
    std::cout << "Stdout of the created process: " << out_str << '\n';
  }
}

#endif
