#include <cu0/env/environment_variable.hh>
#include <cassert>

int main() {
  constexpr auto TEST_KEY = "test_key";
  constexpr auto TEST_VALUE = "test_value";
  constexpr auto ANOTHER_TEST_VALUE = "another_test_value";
  constexpr auto ANOTHER_TEST_VALUE2 = "another_test_value2";
  constexpr auto TEST_VALUE_NEXT = "test_value_next";

  auto uniqueTestKey = std::string{TEST_KEY};
  auto it = 0;

  while (std::getenv(uniqueTestKey.c_str()) != NULL) {
    uniqueTestKey = std::string{TEST_KEY} + '_' + std::to_string(++it);
  }
  setenv(uniqueTestKey.c_str(), TEST_VALUE, false);

  auto environmentVariable = cu0::EnvironmentVariable::unsynced(uniqueTestKey);
  assert(environmentVariable.key() == uniqueTestKey);
  assert(!environmentVariable.cached().has_value());
  {
    const auto synced = environmentVariable.sync();
    assert(synced.has_value());
    assert(synced.value() == TEST_VALUE);
  }
  assert(environmentVariable.key() == uniqueTestKey);
  assert(environmentVariable.cached().has_value());
  assert(environmentVariable.cached().value() == TEST_VALUE);

  auto anotherUniqueTestKey = uniqueTestKey;
  do {
    anotherUniqueTestKey = std::string{TEST_KEY} + '_' + std::to_string(++it);
  } while (std::getenv(anotherUniqueTestKey.c_str()) != NULL);
  setenv(anotherUniqueTestKey.c_str(), ANOTHER_TEST_VALUE, false);

  auto anotherEnvironmentVariable =
      cu0::EnvironmentVariable::synced(anotherUniqueTestKey);
  assert(anotherEnvironmentVariable.key() == anotherUniqueTestKey);
  assert(anotherEnvironmentVariable.cached().has_value());
  assert(anotherEnvironmentVariable.cached().value() == ANOTHER_TEST_VALUE);

  setenv(anotherUniqueTestKey.c_str(), ANOTHER_TEST_VALUE2, true);

  assert(anotherEnvironmentVariable.key() == anotherUniqueTestKey);
  assert(anotherEnvironmentVariable.cached().has_value());
  assert(anotherEnvironmentVariable.cached().value() == ANOTHER_TEST_VALUE);
  assert(anotherEnvironmentVariable.sync().has_value());
  assert(anotherEnvironmentVariable.sync().value() == ANOTHER_TEST_VALUE2);
  assert(anotherEnvironmentVariable.key() == anotherUniqueTestKey);
  assert(anotherEnvironmentVariable.cached().has_value());
  assert(anotherEnvironmentVariable.cached().value() == ANOTHER_TEST_VALUE2);

  assert(environmentVariable.key() == uniqueTestKey);
  assert(environmentVariable.cached().has_value());
  assert(environmentVariable.cached().value() == TEST_VALUE);
  assert(environmentVariable.sync().has_value());
  assert(environmentVariable.sync().value() == TEST_VALUE);

#ifdef __unix__
  assert(environmentVariable.set(TEST_VALUE_NEXT) ==
      cu0::EnvironmentVariable::SetError::NO_ERROR
  );
  assert(environmentVariable.key() == uniqueTestKey);
  assert(environmentVariable.cached().has_value());
  assert(environmentVariable.cached().value() == TEST_VALUE_NEXT);
  assert(environmentVariable.sync().has_value());
  assert(environmentVariable.sync().value() == TEST_VALUE_NEXT);

  assert(environmentVariable.unset() ==
      cu0::EnvironmentVariable::SetError::NO_ERROR
  );
  assert(environmentVariable.key() == uniqueTestKey);
  assert(!environmentVariable.cached().has_value());
  assert(!environmentVariable.sync().has_value());
#endif

  return 0;
}
