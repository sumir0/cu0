#include <cu0/env/environment_variable.hh>
#include <iostream>

int main() {
  //! @note an environment variable may not be set
  //! @note read value of the environment variable with the key 'KEY'
  const auto environmentVariable = cu0::EnvironmentVariable::synced("KEY");
  //! @note if the environment variable is not set ->
  //!     an empty optional is returned
  //! @note cached contains the value of the environment variable at the time of
  //!     last sync with the environment @see EnvironmentVariable::sync
  const auto& cached = environmentVariable.cached();
  std::cout << "Value of the environment variable 'KEY': " <<
      (cached.has_value() ? cached.value() : "<not-set>") << '\n';
}
