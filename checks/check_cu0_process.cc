#include <cu0/proc/process.hh>
#include <cassert>

int main(int argc, char** argv) {

#ifdef __unix__
#if __has_include(<unistd.h>)
  const auto thisProcess = cu0::Process::current();
  assert(thisProcess.pid() == getpid());
#else
#warning <unistd.h> is not found => cu0::Process::current() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::current() will not be checked
#endif

  struct ProcessCheck : public cu0::Process {
    constexpr ProcessCheck() = default;
  };
  const auto someProcess = ProcessCheck{};
  assert(someProcess.pid() == 0);

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
#warning <unistd.h> is not found => cu0::Process::create() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::create() will not be checked
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  assert(!thisProcess.exitCode().has_value());
#if __has_include(<unistd.h>)
  while (!createdProcess->exitCode().has_value());
  assert(createdProcess->exitCode().value() == ENOENT);
  while (!createdWithArgumentsProcess->exitCode().has_value());
  assert(createdWithArgumentsProcess->exitCode().value() == ENOENT);
  while (!createdWithEnvironmentProcess->exitCode().has_value());
  assert(createdWithEnvironmentProcess->exitCode().value() == ENOENT);
  while (!createdWithArgumentsAndEnvironmentProcess->exitCode().has_value());
  assert(
      createdWithArgumentsAndEnvironmentProcess->exitCode().value() == ENOENT
  );

  //! for subprocess check
  if (argc > 1) {
    return std::stoi(argv[1]);
  }

  const auto executableWithZeroExitCode = cu0::Executable{
    .binary = argv[0],
    .arguments = {"0"},
  };
  const auto processWithZeroExitCode =
      cu0::Process::create(executableWithZeroExitCode);
  assert(processWithZeroExitCode.has_value());
  while (!processWithZeroExitCode->exitCode().has_value());
  assert(processWithZeroExitCode->exitCode().value() == 0);

  const auto executableWithOneExitCode = cu0::Executable{
    .binary = argv[0],
    .arguments = {"1"},
  };
  const auto processWithOneExitCode =
      cu0::Process::create(executableWithOneExitCode);
  assert(processWithOneExitCode.has_value());
  while (!processWithOneExitCode->exitCode().has_value());
  assert(processWithOneExitCode->exitCode().value() == 1);
#else
#warning <unistd.h> is not found => \
    cu0::Process::exitCode() will not be checked for created processes
#endif
#else
#warning <sys/types.h> or <sys/wait.h> is not found => \
    cu0::Process::exitCode() will not be checked
#endif
#endif

  return 0;
}
