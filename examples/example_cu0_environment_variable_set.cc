#include <cu0/env/environment_variable.hh>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported ->
//!         a compile-time warning will be present
//!     else (if all features are supported) ->
//!         no feature-related compile-time warnings will be present
#ifndef __unix__
#warning __unix__ is not defined => \
    cu0::EnvironmentVariable::set(std::string) will not be used in the example
int main() {}
#else

int main() {
  //! @note create an environment variable without syncing with the environment
  //!     because we will set the value of the environment variable later
  //!     no syncing before that is needed
  auto environmentVariable = cu0::EnvironmentVariable::unsynced("KEY");
  //! @note set the value of the environment variable
  //! @note automatically updates the environment with the value
  environmentVariable.set("some_value");
  //! @note access cached value of the environment variable
  const auto& cached = environmentVariable.cached();
  std::cout << "Value of the environment variable 'KEY': " <<
      (cached.has_value() ? cached.value() : "<not-set>") << '\n';
}

#endif
