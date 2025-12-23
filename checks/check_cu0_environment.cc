#include <cu0/env/environment.hh>
#include <cassert>

#if __has_include(<unistd.h>)
#include <cstdio>
#include <set>
#include <string>
#endif

#if __has_include(<unistd.h>)
extern char** environ;
#endif

int main() {
#if __has_include(<unistd.h>)
  {
    const auto& environment =
        cu0::Environment::as<std::map<std::string, std::string>>();
    auto set = std::set<std::string>{};
    for (auto&& [key, value] : environment) {
      set.insert(key + "=" + value);
    }
    for (char** envp = environ; *envp != NULL; envp++) {
      const auto actual_string = std::string{*envp};
      assert(set.contains(actual_string));
    }
  }
  {
    const auto& environment =
        cu0::Environment::as<std::vector<cu0::EnvironmentVariable>>();
    auto set = std::set<std::string>{};
    for (const auto& variable : environment) {
      set.insert(variable.key() + "=" + variable.cached().value());
    }
    for (char** envp = environ; *envp != NULL; envp++) {
      const auto actual_string = std::string{*envp};
      assert(set.contains(actual_string));
    }
  }
#else
#warning <unistd.h> is not found => \
template <class T> cu0::Environment::as<T>() will not be checked
#endif
}
