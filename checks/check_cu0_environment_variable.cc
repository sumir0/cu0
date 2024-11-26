#include <cu0/env/environment_variable.hh>
#include <stdlib.h>
#include <cassert>

int main() {
  constexpr auto TEST_KEY = "test_key";
  constexpr auto TEST_VALUE = "test_value";
  constexpr auto ANOTHER_TEST_VALUE = "another_test_value";
  constexpr auto ANOTHER_TEST_VALUE2 = "another_test_value2";

  auto uniqueTestKey = std::string{TEST_KEY};
  auto it = 0;

  while (std::getenv(uniqueTestKey.c_str()) != NULL) {
    uniqueTestKey = std::string{TEST_KEY} + '_' + std::to_string(++it);
  }
  setenv(uniqueTestKey.c_str(), TEST_VALUE, false);

  auto environmentVariable = cu0::EnvironmentVariable{uniqueTestKey};
  assert(environmentVariable.key() == uniqueTestKey);
  assert(environmentVariable.cachedValue() == TEST_VALUE);

  auto anotherUniqueTestKey = uniqueTestKey;
  do {
    anotherUniqueTestKey = std::string{TEST_KEY} + '_' + std::to_string(++it);
  } while (std::getenv(anotherUniqueTestKey.c_str()) != NULL);
  setenv(anotherUniqueTestKey.c_str(), ANOTHER_TEST_VALUE, false);

  auto anotherEnvironmentVariable =
      cu0::EnvironmentVariable{anotherUniqueTestKey};
  assert(anotherEnvironmentVariable.key() == anotherUniqueTestKey);
  assert(anotherEnvironmentVariable.cachedValue() == ANOTHER_TEST_VALUE);

  setenv(anotherUniqueTestKey.c_str(), ANOTHER_TEST_VALUE2, true);

  assert(anotherEnvironmentVariable.key() == anotherUniqueTestKey);
  assert(anotherEnvironmentVariable.cachedValue() == ANOTHER_TEST_VALUE);
  assert(anotherEnvironmentVariable.sync() == ANOTHER_TEST_VALUE2);
  assert(anotherEnvironmentVariable.key() == anotherUniqueTestKey);
  assert(anotherEnvironmentVariable.cachedValue() == ANOTHER_TEST_VALUE2);

  assert(environmentVariable.key() == uniqueTestKey);
  assert(environmentVariable.cachedValue() == TEST_VALUE);
  assert(environmentVariable.sync() == TEST_VALUE);

  return 0;
}
