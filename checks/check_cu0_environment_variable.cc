#include <cu0/env/environment_variable.hh>
#include <stdlib.h>
#include <cassert>

int main() {
  constexpr auto TEST_KEY = "test_key";
  constexpr auto TEST_VALUE = "test_value";
  constexpr auto ANOTHER_TEST_VALUE = "another_test_value";

  auto uniqueTestKey = std::string{TEST_KEY};
  auto it = 0;

  while (std::getenv(uniqueTestKey.c_str()) != NULL) {
    uniqueTestKey = std::string{TEST_KEY} + '_' + std::to_string(++it);
  }
  setenv(uniqueTestKey.c_str(), TEST_VALUE, false); //! do not replace

  const auto environmentVariable = cu0::EnvironmentVariable{uniqueTestKey};
  assert(environmentVariable.cachedValue() == TEST_VALUE);

  do {
    uniqueTestKey = std::string{TEST_KEY} + '_' + std::to_string(++it);
  } while (std::getenv(uniqueTestKey.c_str()) != NULL);
  setenv(uniqueTestKey.c_str(), ANOTHER_TEST_VALUE, false); //! do not replace

  const auto anotherEnvironmentVariable =
      cu0::EnvironmentVariable{uniqueTestKey};
  assert(anotherEnvironmentVariable.cachedValue() == ANOTHER_TEST_VALUE);

  assert(environmentVariable.cachedValue() == TEST_VALUE);

  return 0;
}
