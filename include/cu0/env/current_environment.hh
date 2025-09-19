#ifndef CU0_CURRENT_ENVIRONMENT_HH_
#define CU0_CURRENT_ENVIRONMENT_HH_

#if __has_include(<unistd.h>)
#include <unistd.h>
#include <map>
#include <string>
#include <vector>
#endif

#if __has_include(<unistd.h>)
#include <cu0/env/environment_variable.hh>
#endif

#if __has_include(<unistd.h>)
extern char** environ;
#endif

namespace cu0 {

struct CurrentEnvironment {
  /*!
   * @brief accesses current environment and returns it as a specified type
   * @note this function is marked deleted to allow only specializations usage
   * @return current environment as Return type
   */
  template <class Return>
  [[nodiscard]]
  constexpr static Return as() = delete;
#if __has_include(<unistd.h>)
  /*!
   * @note specilization of template <class Return> static Return as()
   * @brief accesses current environment
   * @return map of the current environment variables in the form <key, value>
   */
  template <>
  [[nodiscard]]
  std::map<std::string, std::string> as();
#endif
#if __has_include(<unistd.h>)
  /*!
   * @note specilization of template <class Return> static Return as()
   * @brief accesses current environment
   * @note the behaviour is undefined if environment changes during function
   *     execution
   * @return vector of the current environment variables
   */
  template <>
  [[nodiscard]]
  std::vector<EnvironmentVariable> as();
#endif
};

} /// namespace cu0

namespace cu0 {

#if __has_include(<unistd.h>)
template <>
inline std::map<std::string, std::string> CurrentEnvironment::as() {
  auto ret = std::map<std::string, std::string>{};
  for (char** envp = environ; *envp != NULL; envp++) {
    const auto string = std::string{*envp};
    constexpr auto DELIMETER = '=';
    const auto delimeterPos = string.find(DELIMETER);
    const auto key = string.substr(0, delimeterPos);
    const auto value = string.substr(delimeterPos + 1);
    ret.insert({ key, value, });
  }
  return ret;
}
#endif

#if __has_include(<unistd.h>)
template <>
inline std::vector<EnvironmentVariable> CurrentEnvironment::as() {
  auto ret = std::vector<EnvironmentVariable>{};
  for (char** envp = environ; *envp != NULL; envp++) {
    const auto string = std::string{*envp};
    constexpr auto DELIMETER = '=';
    const auto delimeterPos = string.find(DELIMETER);
    const auto key = string.substr(0, delimeterPos);
    const auto value = string.substr(delimeterPos + 1);
    ret.push_back(EnvironmentVariable::unsynced({ key, value, }));
  }
  return ret;
}
#endif

} /// namespace cu0

#endif /// CU0_CURRENT_ENVIRONMENT_HH_
