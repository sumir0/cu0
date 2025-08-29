#include <cu0/env/current_environment.hh>
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
  const auto& environment = cu0::currentEnvironment();
  auto set = std::set<std::string>{};
  for (auto&& [key, value] : environment) {
    set.insert(key + "=" + value);
  }
  for (char** envp = environ; *envp != NULL; envp++) {
    const auto actualString = std::string{*envp};
    assert(set.contains(actualString));
  }
#else
#warning <unistd.h> is not found => \
    cu0::currentEnvironment() will not be checked
#endif
}
