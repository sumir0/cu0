#include <cu0/env/environment_variable.hh>
#include <iostream>

int main() {
  //! @note environment variable may not be set
  auto environmentVariable = cu0::EnvironmentVariable::unsynced("KEY");
  //! @note wait for external environment variable change and
  //!     sync value of the environment variable with the key 'KEY'
  const auto& synced = environmentVariable.sync();
  std::cout << "Synced value of the environment variable 'KEY': " <<
      (synced.has_value() ? synced.value() : "<not-set>") << '\n';
}
