#include <cu0/env/environment_variable.hh>
#include <cassert>

int main() {
  constexpr auto TEST_KEY = "test_key";
  constexpr auto TEST_VALUE = "test_value";
  constexpr auto ANOTHER_TEST_VALUE = "another_test_value";
  constexpr auto ANOTHER_TEST_VALUE2 = "another_test_value2";
  constexpr auto TEST_VALUE_NEXT = "test_value_next";

  auto unique_test_key = std::string{TEST_KEY};
  auto it = 0;

  while (std::getenv(unique_test_key.c_str()) != NULL) {
    unique_test_key = std::string{TEST_KEY} + '_' + std::to_string(++it);
  }
  setenv(unique_test_key.c_str(), TEST_VALUE, false);

  auto environment_variable =
      cu0::EnvironmentVariable::unsynced(unique_test_key);
  assert(environment_variable.key() == unique_test_key);
  assert(!environment_variable.cached().has_value());
  {
    const auto synced = environment_variable.sync();
    assert(synced.has_value());
    assert(synced.value() == TEST_VALUE);
  }
  assert(environment_variable.key() == unique_test_key);
  assert(environment_variable.cached().has_value());
  assert(environment_variable.cached().value() == TEST_VALUE);

  auto another_unique_test_key = unique_test_key;
  do {
    another_unique_test_key =
        std::string{TEST_KEY} + '_' + std::to_string(++it);
  } while (std::getenv(another_unique_test_key.c_str()) != NULL);
  setenv(another_unique_test_key.c_str(), ANOTHER_TEST_VALUE, false);

  auto another_environment_variable =
      cu0::EnvironmentVariable::synced(another_unique_test_key);
  assert(another_environment_variable.key() == another_unique_test_key);
  assert(another_environment_variable.cached().has_value());
  assert(another_environment_variable.cached().value() == ANOTHER_TEST_VALUE);

  setenv(another_unique_test_key.c_str(), ANOTHER_TEST_VALUE2, true);

  assert(another_environment_variable.key() == another_unique_test_key);
  assert(another_environment_variable.cached().has_value());
  assert(another_environment_variable.cached().value() == ANOTHER_TEST_VALUE);
  assert(another_environment_variable.sync().has_value());
  assert(another_environment_variable.sync().value() == ANOTHER_TEST_VALUE2);
  assert(another_environment_variable.key() == another_unique_test_key);
  assert(another_environment_variable.cached().has_value());
  assert(another_environment_variable.cached().value() == ANOTHER_TEST_VALUE2);

  assert(environment_variable.key() == unique_test_key);
  assert(environment_variable.cached().has_value());
  assert(environment_variable.cached().value() == TEST_VALUE);
  assert(environment_variable.sync().has_value());
  assert(environment_variable.sync().value() == TEST_VALUE);

#if !defined(NOT_AN_X)
  const auto set_variant = environment_variable.set(TEST_VALUE_NEXT);
  assert(std::holds_alternative<std::monostate>(set_variant));
  assert(environment_variable.key() == unique_test_key);
  assert(environment_variable.cached().has_value());
  assert(environment_variable.cached().value() == TEST_VALUE_NEXT);
  assert(environment_variable.sync().has_value());
  assert(environment_variable.sync().value() == TEST_VALUE_NEXT);

  const auto unset_variant = environment_variable.unset();
  assert(std::holds_alternative<std::monostate>(unset_variant));
  assert(environment_variable.key() == unique_test_key);
  assert(!environment_variable.cached().has_value());
  assert(!environment_variable.sync().has_value());
#endif

  unsetenv(unique_test_key.c_str());
  unsetenv(another_unique_test_key.c_str());

  return 0;
}
