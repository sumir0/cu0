#include <cu0/proc/process.hh>
#include <cassert>
#include <atomic>
#include <thread>

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
  auto createdProcess = cu0::Process::create(executable);
  assert(createdProcess.has_value());
  assert(createdProcess->pid() != 0);

  const auto executableWithArguments = cu0::Executable{
    .arguments = { "arg1", "arg2", "arg3", },
  };
  auto createdWithArgumentsProcess =
      cu0::Process::create(executableWithArguments);
  assert(createdWithArgumentsProcess.has_value());
  assert(createdWithArgumentsProcess->pid() != 0);

  const auto executableWithEnvironment = cu0::Executable{
    .environment = { { "k1", "v1", }, { "k2", "v2", }, { "k3", "v3", } },
  };
  auto createdWithEnvironmentProcess =
      cu0::Process::create(executableWithEnvironment);
  assert(createdWithEnvironmentProcess.has_value());
  assert(createdWithEnvironmentProcess->pid() != 0);

  const auto executableWithArgumentsAndEnvironment = cu0::Executable{
    .arguments = { "arg1", "arg2", "arg3", },
    .environment = { { "k1", "v1", }, { "k2", "v2", }, { "k3", "v3", } },
  };
  auto createdWithArgumentsAndEnvironmentProcess =
      cu0::Process::create(executableWithArgumentsAndEnvironment);
  assert(createdWithArgumentsAndEnvironmentProcess.has_value());
  assert(createdWithArgumentsAndEnvironmentProcess->pid() != 0);
#else
#warning <unistd.h> is not found => cu0::Process::create() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::create() will not be checked
#endif

  //! for subprocess check
  if (argc > 1) {
    return std::stoi(argv[1]);
  }

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  const auto executableWithExitCodeTwo = cu0::Executable{
    .binary = argv[0],
    .arguments = {"2"},
  };
  auto processWithExitCodeTwo =
      cu0::Process::create(executableWithExitCodeTwo);
  assert(processWithExitCodeTwo.has_value());
  std::atomic<bool> waitFail = true;
  auto waitThread = std::thread([&processWithExitCodeTwo, &waitFail](){
    assert(&processWithExitCodeTwo->wait() == &(*processWithExitCodeTwo));
    waitFail = false;
  });
  const auto waitThreadStartTime = std::chrono::high_resolution_clock::now();
  while (
      waitFail &&
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::high_resolution_clock::now() - waitThreadStartTime
      ) < std::chrono::seconds{32}
  ) {
    //! do nothing, wait
  }
  if (waitThread.joinable()) {
    waitThread.join();
  }
  assert(!waitFail);
  assert(&processWithExitCodeTwo->wait() == &(*processWithExitCodeTwo));
  assert(processWithExitCodeTwo->exitCode().has_value());
  assert(processWithExitCodeTwo->exitCode().value() == 2);
#else
#warning <sys/types.h> or <sys/wait.h> is not found => \
    cu0::Process::wait() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::wait() will not be checked
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  assert(!thisProcess.exitCode().has_value());
#if __has_include(<unistd.h>)
  createdProcess->wait();
  assert(createdProcess->exitCode().value() == ENOENT);
  createdWithArgumentsProcess->wait();
  assert(createdWithArgumentsProcess->exitCode().value() == ENOENT);
  createdWithEnvironmentProcess->wait();
  assert(createdWithEnvironmentProcess->exitCode().value() == ENOENT);
  createdWithArgumentsAndEnvironmentProcess->wait();
  assert(
      createdWithArgumentsAndEnvironmentProcess->exitCode().value() == ENOENT
  );

  const auto executableWithExitCodeZero = cu0::Executable{
    .binary = argv[0],
    .arguments = {"0"},
  };
  auto processWithExitCodeZero =
      cu0::Process::create(executableWithExitCodeZero);
  assert(processWithExitCodeZero.has_value());
  processWithExitCodeZero->wait();
  assert(processWithExitCodeZero->exitCode().value() == 0);

  const auto executableWithExitCodeOne = cu0::Executable{
    .binary = argv[0],
    .arguments = {"1"},
  };
  auto processWithExitCodeOne =
      cu0::Process::create(executableWithExitCodeOne);
  assert(processWithExitCodeOne.has_value());
  processWithExitCodeOne->wait();
  assert(processWithExitCodeOne->exitCode().value() == 1);
#else
#warning <unistd.h> is not found => \
    cu0::Process::exitCode() will not be checked for created processes
#endif
#else
#warning <sys/types.h> or <sys/wait.h> is not found => \
    cu0::Process::exitCode() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::exitCode() will not be checked
#endif

  return 0;
}
