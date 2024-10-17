#ifndef CU0_ENVIRONMENT_VARIABLE_HH_
#define CU0_ENVIRONMENT_VARIABLE_HH_

#include <cstdlib>
#include <string>

namespace cu0 {

/*!
 * @brief The EnvironmentVariable struct provides a way to
 *     access environment variable values
 * @see EnvironmentVariable::EnvironmentVariable(const std::string& key)
 * @see EnvironmentVariable::cachedValue() const
 */
struct EnvironmentVariable {
public:
  /*!
   * @brief destructor that may be used in inherited structs
   */
  virtual constexpr ~EnvironmentVariable() = default;
  /*!
   * @brief constructs an instance using an environment variable key
   * @note accesses the actual environment variable value
   * @param key is the environment variable key
   */
  EnvironmentVariable(const std::string& key);
  /*!
   * @brief accesses cached value of the associated environment variable
   * @note cached value may not represent the associated environment variable if
   *     it was modified after the construction of this instance and
   *     before a call to this function
   * @return cached value as a const reference
   */
  constexpr const std::string& cachedValue() const;
protected:
  /*!
   * @brief helper function to get value of an environment variable by a key
   * @param key is the environment variable key
   * @return
   *     if the environment variable exists -> the value of it as a string
   *     else -> an empty string
   */
  static std::string getBy(const std::string& key);
  /*!
   * @brief constructor that may be used in inherited structs
   */
  constexpr EnvironmentVariable() = default;
  //! cached value of the associted environment variable
  std::string value_{};
};

} /// namespace cu0

namespace cu0 {

inline EnvironmentVariable::EnvironmentVariable(const std::string& key)
  : value_{EnvironmentVariable::getBy(key)} {}

constexpr const std::string& EnvironmentVariable::cachedValue() const {
  return this->value_;
}

inline std::string EnvironmentVariable::getBy(const std::string& key) {
  const auto* raw = std::getenv(key.c_str());
  return raw == NULL ? std::string{} : std::string{raw};
}

} /// namespace cu0

#endif /// CU0_ENVIRONMENT_VARIABLE_HH_
