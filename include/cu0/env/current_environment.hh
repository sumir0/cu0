#ifndef CU0_CURRENT_ENVIRONMENT_HH_
#define CU0_CURRENT_ENVIRONMENT_HH_

#if __has_include(<unistd.h>)
#include <unistd.h>
#include <map>
#include <string>
#endif

#if __has_include(<unistd.h>)
extern char** environ;
#endif

namespace cu0 {

#if __has_include(<unistd.h>)
/*!
 * @brief gets current environment
 * @return map of the current environment variables in form <key, value>
 */
std::map<std::string, std::string> currentEnvironment();
#endif

} /// namespace cu0

namespace cu0 {

#if __has_include(<unistd.h>)
inline std::map<std::string, std::string> currentEnvironment() {
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

} /// namespace cu0

#endif /// CU0_CURRENT_ENVIRONMENT_HH_
