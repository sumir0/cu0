#ifndef CU0_ENVIRONMENT_VARIABLE_HH_
#define CU0_ENVIRONMENT_VARIABLE_HH_

#include <cstdlib>
#include <optional>
#include <string>

// #ifdef __unix__
// #if __has_include(<stdlib.h>)
// #include <stdlib.h>
// #endif
// #endif

namespace cu0 {

/*!
 * @brief The EnvironmentVariable struct provides a way to
 *     access environment variable values
 * @see EnvironmentVariable::unsynced(std::string key)
 * @see EnvironmentVariable::synced(std::string key)
 * @see EnvironmentVariable::cached() const
 */
struct EnvironmentVariable {
public:
#ifdef __unix__
  /*!
   * @brief enum of possible errors during value setting
   */
  enum class SetError {
    NO_ERROR = 0, //! no error
    INVALID, //! key_ is a string of length 0, or contains an '=' character
    MEMORY, //! insufficient memory to add a new variable to the environment
  };
#endif
  /*!
   * @brief destructor that may be used in inherited structs
   */
  virtual constexpr ~EnvironmentVariable() = default;
  /*!
   * @brief creates an instance without syncing with the environment
   * @param key is the environment variable key
   * @return unsynced environment variable
   */
  static constexpr EnvironmentVariable unsynced(std::string key);
  /*!
   * @brief creates an instance syncing with the environment
   * @param key is the environment variable key
   * @return synced environment variable
   */
  static EnvironmentVariable synced(std::string key);
  /*!
   * @brief accesses the key of the associated environment variable
   * @return key as a const reference
   */
  constexpr const std::string& key() const;
  /*!
   * @brief sets the key to the specified value
   * @note also sets the value to the actual environment variable value
   * @param key is the environment variable key
   */
  void key(std::string key);
  /*!
   * @brief accesses cached value of the associated environment variable
   * @note cached value may not represent the associated environment variable if
   *     it was modified after the construction of this instance and
   *     before a call to this function
   * @return cached value as a const reference
   */
  constexpr const std::optional<std::string>& cached() const;
  /*!
   * @brief syncs cached value to the actual value of the
   *     associated environment variable
   * @return new cached value as a const reference
   */
  const std::optional<std::string>& sync();
#ifdef __unix__
  /*!
   * @brief sets the value of the associated environment variable
   * @param value is the value to be set
   * @return error code @see SetError
   */
  SetError set(std::string value);
#endif
#ifdef __unix__
  /*!
   * @brief unsets the value of the associated environment variable
   * @return error code @see SetError
   */
  SetError unset();
#endif
protected:
  /*!
   * @tparam ConvertTo is a struct conversion to which is needed
   * @note ConvertTo may be one of { SetError, }
   * @brief converts an error number (error code) to the ConvertTo type
   * @param errorNumber is the error number (error code)
   * @return converted value
   */
#ifdef __unix__
  template <class ConvertTo>
  static constexpr ConvertTo convert(const int& errorNumber);
#endif
  /*!
   * @brief constructs an instance without syncing with the environment
   * @note does not access an actual environment variable value
   */
  constexpr EnvironmentVariable() = default;
  //! key of the environment variable to associate this isntance with
  std::string key_{};
  //! cached value of the associted environment variable
  std::optional<std::string> value_{};
};

#ifdef __unix__
template <>
constexpr EnvironmentVariable::SetError
EnvironmentVariable::convert(const int& errorNumber);
#endif

} /// namespace cu0

namespace cu0 {

constexpr EnvironmentVariable EnvironmentVariable::unsynced(std::string key) {
  auto ret = EnvironmentVariable{};
  ret.key_ = std::move(key);
  return ret;
}

inline EnvironmentVariable EnvironmentVariable::synced(std::string key) {
  auto ret = EnvironmentVariable::unsynced(std::move(key));
  ret.sync();
  return ret;
}

constexpr const std::string& EnvironmentVariable::key() const {
  return this->key_;
}

inline void EnvironmentVariable::key(std::string key) {
  this->key_ = std::move(key);
}

constexpr const std::optional<std::string>&
EnvironmentVariable::cached() const {
  return this->value_;
}

inline const std::optional<std::string>& EnvironmentVariable::sync() {
  const auto* raw = std::getenv(this->key_.c_str());
  return this->value_ =
      (raw == NULL ? std::optional<std::string>{} : std::string{raw});
}

#ifdef __unix__
inline EnvironmentVariable::SetError EnvironmentVariable::set(
    std::string value
) {
  if (setenv(
      this->key_.c_str(),
      value.c_str(),
      true //! replace
  ) == 0) { //! the environment variable is set
    this->value_ = std::move(value);
    return SetError::NO_ERROR;
  } else { //! the environment variable is not set
    return EnvironmentVariable::convert<SetError>(errno);
  }
}
#endif

#ifdef __unix__
inline EnvironmentVariable::SetError EnvironmentVariable::unset() {
  if (unsetenv(this->key_.c_str()) == 0) {
    this->value_ = {};
    return SetError::NO_ERROR;
  } else {
    return EnvironmentVariable::convert<SetError>(errno);
  }
}
#endif

#ifdef __unix__
template <>
constexpr EnvironmentVariable::SetError EnvironmentVariable::convert(
    const int& errorNumber
) {
  switch (errorNumber) {
  case EINVAL:
    return SetError::INVALID;
  case ENOMEM:
    return SetError::MEMORY;
  default:
    return SetError::NO_ERROR;
  }
}
#endif

} /// namespace cu0

#endif /// CU0_ENVIRONMENT_VARIABLE_HH_
