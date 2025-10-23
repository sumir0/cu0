#ifndef CU0_ENVIRONMENT_HH__
#define CU0_ENVIRONMENT_HH__

#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
cu0::Environment::as() will not be supported
#endif

#if __has_include(<unistd.h>)
#include <functional>
#include <map>
#include <string>
#include <vector>
#endif

#if __has_include(<unistd.h>)
#include <unistd.h>
#endif

#if __has_include(<unistd.h>)
#include <cu0/env/environment_variable.hh>
#endif

#if __has_include(<unistd.h>)
extern char** environ;
#endif

namespace cu0 {

/*!
 * @brief The Environment struct provides a way to access the environment
 * @note
 *     no instances of this struct can be created
 *     for a way to access the environment of execution @see Environment::as()
 */
struct Environment {
public:
  //! no instances of this struct can be created
  constexpr Environment() = delete;
  /*!
   * @brief accesses the environment of execution and returns it as a
   *     specified type
   * @note this function is marked deleted to allow only usage of
   *     specializations
   * @tparam Return is the type that will be returned
   * @return environment as Return type
   */
  template <class Return>
  [[nodiscard]]
  constexpr static Return as() = delete;
#if __has_include(<unistd.h>)
  /*!
   * @note specilization of Environment::as()
   * @brief copies environment variables into std::map<std::string, std::string>
   * @note the behaviour is undefined if environment changes during function
   *     execution
   * @return map of the current environment variables in the form <key, value>
   */
  template <>
  [[nodiscard]]
  std::map<std::string, std::string> as();
#endif
#if __has_include(<unistd.h>)
  /*!
   * @note specilization of Environment::as()
   * @brief copies environment variables into std::vector<EnvironmentVariable>
   * @note the behaviour is undefined if environment changes during function
   *     execution
   * @return vector of the current environment variables
   */
  template <>
  [[nodiscard]]
  std::vector<EnvironmentVariable> as();
#endif
protected:
#if __has_include(<unistd.h>)
  /*!
   * @brief accesses the environment of execution and returns it as a
   *     specified type
   * @tparam Return is the type that will be returned
   * @param insert is the function to insert key-value data
   *     Return& param is the first parameter of the function into which
   *         key-value data needs to be inserted
   *     std::string&& is the second parameter of the function representing key
   *         of the environment variable
   *     std::string&& is the third parameter of the function representing value
   *         of the environment variable
   * @return environment as Return type
   */
  template <class Return>
  [[nodiscard]]
  constexpr static Return as(
      const std::function<void(Return&, std::string&&, std::string&&)>& insert
  );
#endif
private:
};

} /// namespace cu0

namespace cu0 {

#if __has_include(<unistd.h>)
template <>
inline std::map<std::string, std::string> Environment::as() {
  return Environment::as<std::map<std::string, std::string>>([](
      std::map<std::string, std::string>& ret,
      std::string&& key,
      std::string&& value
  ){
    ret.insert({ std::move(key), std::move(value), });
  });
}
#endif

#if __has_include(<unistd.h>)
template <>
inline std::vector<EnvironmentVariable> Environment::as() {
  return Environment::as<std::vector<EnvironmentVariable>>([](
      std::vector<EnvironmentVariable>& ret,
      std::string&& key,
      std::string&& value
  ){
    ret.push_back(EnvironmentVariable::unsynced({ key, value, }));
  });
}
#endif

#if __has_include(<unistd.h>)
template <class Return>
constexpr Return Environment::as(
    const std::function<void(Return&, std::string&&, std::string&&)>& insert
) {
  auto ret = Return{};
  for (char** envp = environ; *envp != NULL; envp++) {
    const auto string = std::string{*envp};
    constexpr auto DELIMETER = '=';
    const auto delimeterPos = string.find(DELIMETER);
    auto key = string.substr(0, delimeterPos);
    auto value = string.substr(delimeterPos + 1);
    insert(ret, std::move(key), std::move(value));
  }
  return ret;
}
#endif

} /// namespace cu0

#endif /// CU0_ENVIRONMENT_HH__
