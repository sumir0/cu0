#ifndef CU0_ENVIRONMENT_VARIABLE_HH_
#define CU0_ENVIRONMENT_VARIABLE_HH_

#include <cstdlib>
#include <optional>
#include <string>

namespace cu0 {

/*!
 * @brief The EnvironmentVariableData struct provides a way to represent
 *     environment variables in a memory
 */
struct EnvironmentVariableData {
  //! key of the associated environment variable
  std::string key{};
  //! value of the associated environment variable
  std::optional<std::string> value{};
};

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
    //! data_.key is a string of length 0, or contains an '=' character
    INVALID = EINVAL,
    //! insufficient memory to add a new variable to the environment
    MEMORY = ENOMEM,
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
  [[nodiscard]]
  static constexpr EnvironmentVariable unsynced(std::string key);
  /*!
   * @brief creates an instance without syncing with the environment
   * @param data is the environment variable data, i.e. key and value
   * @return unsynced environment variable with cached value from the data
   */
  [[nodiscard]]
  static constexpr EnvironmentVariable unsynced(EnvironmentVariableData data);
  /*!
   * @brief creates an instance syncing with the environment
   * @param key is the environment variable key
   * @return synced environment variable
   */
  [[nodiscard]]
  static EnvironmentVariable synced(std::string key);
  /*!
   * @brief accesses the data of this instance @see data_
   * @return data as a const reference
   */
  [[nodiscard]]
  constexpr const EnvironmentVariableData& data() const;
  /*!
   * @brief accesses the key of an associated environment variable
   * @return key as a const reference
   */
  [[nodiscard]]
  constexpr const std::string& key() const;
  /*!
   * @brief accesses the cached value of the associated environment variable
   * @note the cached value may not represent the associated environment
   *     variable if it was modified after the construction of this instance and
   *     before a call to this function
   * @return cached value as a const reference
   */
  [[nodiscard]]
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
#ifdef __unix__
  /*!
   * @tparam ConvertTo is a struct conversion to which is needed
   * @note ConvertTo may be one of { SetError, }
   * @brief converts an error number (error code) to the ConvertTo type
   * @param errorNumber is the error number (error code)
   * @return converted value
   */
  template <class ConvertTo>
  static constexpr ConvertTo convert(const int& errorNumber);
#endif
  /*!
   * @brief constructs an instance without syncing with the environment
   * @note does not access an actual environment variable value
   */
  constexpr EnvironmentVariable() = default;
  //! key-value data of the associated environment variable
  EnvironmentVariableData data_{};
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
  ret.data_.key = std::move(key);
  return ret;
}

constexpr EnvironmentVariable EnvironmentVariable::unsynced(
    EnvironmentVariableData data
) {
  auto ret = EnvironmentVariable{};
  ret.data_ = std::move(data);
  return ret;
}

inline EnvironmentVariable EnvironmentVariable::synced(std::string key) {
  auto ret = EnvironmentVariable::unsynced(std::move(key));
  ret.sync();
  return ret;
}

constexpr const EnvironmentVariableData& EnvironmentVariable::data() const {
  return this->data_;
}

constexpr const std::string& EnvironmentVariable::key() const {
  return this->data_.key;
}

constexpr const std::optional<std::string>&
EnvironmentVariable::cached() const {
  return this->data_.value;
}

inline const std::optional<std::string>& EnvironmentVariable::sync() {
  const auto* raw = std::getenv(this->data_.key.c_str());
  return this->data_.value =
      (raw == NULL ? std::optional<std::string>{} : std::string{raw});
}

#ifdef __unix__
inline EnvironmentVariable::SetError EnvironmentVariable::set(
    std::string value
) {
  if (setenv(
      this->data_.key.c_str(),
      value.c_str(),
      true //! replace
  ) == 0) { //! the environment variable is set
    this->data_.value = std::move(value);
    return SetError::NO_ERROR;
  } else { //! the environment variable is not set
    return EnvironmentVariable::convert<SetError>(errno);
  }
}
#endif

#ifdef __unix__
inline EnvironmentVariable::SetError EnvironmentVariable::unset() {
  if (unsetenv(this->data_.key.c_str()) == 0) {
    this->data_.value = {};
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
  return static_cast<SetError>(errorNumber);
}
#endif

} /// namespace cu0

#endif /// CU0_ENVIRONMENT_VARIABLE_HH_
