#include <cu0/proc/process.hh>
#include <cassert>

int main() {
#ifdef __unix__
#if __has_include(<unistd.h>)
  const auto thisProcess = cu0::Process::current();
  assert(thisProcess.pid() == getpid());
#else
#warning <unistd.h> not found => cu0::Process::current() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::current() will not be checked
#endif
  struct ProcessCheck : public cu0::Process {
    constexpr ProcessCheck() = default;
  };
  constexpr auto someProcess = ProcessCheck{};
  static_assert(someProcess.pid() == 0);

  return 0;
}
