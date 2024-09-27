#include <cu0/env.hxx>
#include <iostream>

int main() {
  //! @note environment variable may not be set
  //! read value of the environment variable with the key 'KEY'
  //! @note if the environment variable is not set ->
  //!     empty string is returned
  //! @note if the returned cached value is empty ->
  //!     it does not mean that the environment variable was not set,
  //!     it may be set to the empty value
  //! cachedValue contains value of the environment variable at the time of
  //!     cu0::EnvironmentVariable instance construction
  const auto& cachedValue = cu0::EnvironmentVariable{"KEY"}.cachedValue();
  std::cout << "Value of the environment variable 'KEY': " <<
      (cachedValue.empty() ? "<empty-or-not-set>" : cachedValue) << '\n';
}
