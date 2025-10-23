#include <cu0/env/environment.hh>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported =>
//!         a compile-time warning will be present
//!     else (if all features are supported) =>
//!         no feature-related compile-time warnings will be present
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
template <class T> cu0::Environment::as<T>() will not be used in the example
int main() {}
#else

int main() {
  //! @note not supported on all platforms yet
  //! @note environment variable contains current environment's key-value data
  //! @note environment variable is not synchronized with the actual environment
  //!     i.e. environment variable is a snapshot of the actual environment
  const auto& environment =
      cu0::Environment::as<std::map<std::string, std::string>>();
  //! iterate through the snapshot of the actual environment
  for (const auto& [key, value] : environment) {
    std::cout << "environment[" << key << "]" << "=" << value << '\n';
  }
}

#endif
