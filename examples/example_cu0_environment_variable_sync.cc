#include <cu0/env.hxx>
#include <iostream>

int main() {
  //! @note environment variable may not be set
  auto environmentVariable = cu0::EnvironmentVariable{"KEY"};
  //! wait for external environment variable change and
  //! sync value of the environment variable with the key 'KEY'
  const auto& syncedValue = cu0::EnvironmentVariable{"KEY"}.sync();
  std::cout << "Synced value of the environment variable 'KEY': " <<
      (syncedValue.empty() ? "<empty-or-not-set>" : syncedValue) << '\n';
}
