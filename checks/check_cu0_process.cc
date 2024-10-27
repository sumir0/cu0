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

#ifdef __unix__
#if __has_include(<unistd.h>)
  const auto executable = cu0::Executable{}; //! empty executable
  const auto createdProcess = cu0::Process::create(executable);
  assert(createdProcess.has_value());
  assert(createdProcess->pid() != 0);

  const auto executableWithArguments = cu0::Executable{
    .arguments = { "arg1", "arg2", "arg3", },
  };
  const auto createdWithArgumentsProcess =
      cu0::Process::create(executableWithArguments);
  assert(createdWithArgumentsProcess.has_value());
  assert(createdWithArgumentsProcess->pid() != 0);

  const auto executableWithEnvironment = cu0::Executable{
    .environment = { { "k1", "v1", }, { "k2", "v2", }, { "k3", "v3", } },
  };
  const auto createdWithEnvironmentProcess =
      cu0::Process::create(executableWithEnvironment);
  assert(createdWithEnvironmentProcess.has_value());
  assert(createdWithEnvironmentProcess->pid() != 0);

  const auto executableWithArgumentsAndEnvironment = cu0::Executable{
    .arguments = { "arg1", "arg2", "arg3", },
    .environment = { { "k1", "v1", }, { "k2", "v2", }, { "k3", "v3", } },
  };
  const auto createdWithArgumentsAndEnvironmentProcess =
      cu0::Process::create(executableWithArgumentsAndEnvironment);
  assert(createdWithArgumentsAndEnvironmentProcess.has_value());
  assert(createdWithArgumentsAndEnvironmentProcess->pid() != 0);
#else
#warning <unistd.h> not found => cu0::Process::create() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::create() will not be checked
#endif

  return 0;
}
