#include <cu0/proc/process.hh>
#include <cassert>
#include <algorithm>
#include <array>
#include <atomic>
#include <iostream>
#include <thread>

int main(int argc, char** argv) {

#ifdef __unix__
#if __has_include(<unistd.h>)
  const auto thisProcess = cu0::Process::current();
  assert(thisProcess.pid() == static_cast<unsigned>(getpid()));
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
  auto created = cu0::Process::create(executable);
  assert(std::holds_alternative<cu0::Process>(created));
  auto& createdProcess = std::get<cu0::Process>(created);
  assert(createdProcess.pid() != 0);

  const auto executableWithArguments = cu0::Executable{
    .arguments = { "arg1", "arg2", "arg3", },
  };
  auto createdWithArguments =
      cu0::Process::create(executableWithArguments);
  assert(std::holds_alternative<cu0::Process>(createdWithArguments));
  auto& createdWithArgumentsProcess =
      std::get<cu0::Process>(createdWithArguments);
  assert(createdWithArgumentsProcess.pid() != 0);

  const auto executableWithEnvironment = cu0::Executable{
    .environment = { { "k1", "v1", }, { "k2", "v2", }, { "k3", "v3", } },
  };
  auto createdWithEnvironment =
      cu0::Process::create(executableWithEnvironment);
  assert(std::holds_alternative<cu0::Process>(createdWithEnvironment));
  auto& createdWithEnvironmentProcess =
      std::get<cu0::Process>(createdWithEnvironment);
  assert(createdWithEnvironmentProcess.pid() != 0);

  const auto executableWithArgumentsAndEnvironment = cu0::Executable{
    .arguments = { "arg1", "arg2", "arg3", },
    .environment = { { "k1", "v1", }, { "k2", "v2", }, { "k3", "v3", } },
  };
  auto createdWithArgumentsAndEnvironment =
      cu0::Process::create(executableWithArgumentsAndEnvironment);
  assert(std::holds_alternative<cu0::Process>(
      createdWithArgumentsAndEnvironment
  ));
  auto& createdWithArgumentsAndEnvironmentProcess =
      std::get<cu0::Process>(createdWithArgumentsAndEnvironment);
  assert(createdWithArgumentsAndEnvironmentProcess.pid() != 0);
#else
#warning <unistd.h> is not found => cu0::Process::create() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::create() will not be checked
#endif

  constexpr auto SLEEP_DURATION = 8; //! [s]
  //! for subprocess check
  if (argc > 1) {
    if (std::string{argv[1]} == "64") {
      std::string input;
      std::cin >> input;
      std::cout << input;
      std::cerr << input << input;
      return std::stoi(argv[1]);
    } else if (std::string{argv[1]} == "128") {
      std::this_thread::sleep_for(std::chrono::seconds{SLEEP_DURATION});
      return 0;
    }
    std::cout << argv[1];
    std::cerr << argv[1] << argv[1];
    return std::stoi(argv[1]);
  }

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  {
    const auto executableWithExitCodeTwo = cu0::Executable{
      .binary = argv[0],
      .arguments = {"2"},
    };
    auto createdWithExitCodeTwo =
        cu0::Process::create(executableWithExitCodeTwo);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCodeTwo));
    auto& processWithExitCodeTwo =
        std::get<cu0::Process>(createdWithExitCodeTwo);
    std::atomic<bool> waitFail = true;
    auto waitThread = std::thread([&processWithExitCodeTwo, &waitFail](){
      processWithExitCodeTwo.wait();
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
    processWithExitCodeTwo.wait();
    assert(processWithExitCodeTwo.exitCode().has_value());
    assert(processWithExitCodeTwo.exitCode().value() == 2);
    processWithExitCodeTwo.wait();
    assert(processWithExitCodeTwo.exitCode().has_value());
    assert(processWithExitCodeTwo.exitCode().value() == 2);
    assert(
        processWithExitCodeTwo.waitCautious() == cu0::Process::WaitError::CHILD
    );
    assert(processWithExitCodeTwo.exitCode().has_value());
    assert(processWithExitCodeTwo.exitCode().value() == 2);
  }
  {
    const auto executableWithExitCodeTwo = cu0::Executable{
      .binary = argv[0],
      .arguments = {"2"},
    };
    auto createdWithExitCodeTwo =
        cu0::Process::create(executableWithExitCodeTwo);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCodeTwo));
    std::atomic<bool> waitFail = true;
    auto& processWithExitCodeTwo =
        std::get<cu0::Process>(createdWithExitCodeTwo);
    auto waitThread = std::thread([&processWithExitCodeTwo, &waitFail](){
      assert(
          processWithExitCodeTwo.waitCautious() ==
              cu0::Process::WaitError::NO_ERROR
      );
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
    assert(
        processWithExitCodeTwo.waitCautious() == cu0::Process::WaitError::CHILD
    );
    assert(processWithExitCodeTwo.exitCode().has_value());
    assert(processWithExitCodeTwo.exitCode().value() == 2);
    assert(
        processWithExitCodeTwo.waitCautious() == cu0::Process::WaitError::CHILD
    );
    assert(processWithExitCodeTwo.exitCode().has_value());
    assert(processWithExitCodeTwo.exitCode().value() == 2);
    processWithExitCodeTwo.wait();
    assert(processWithExitCodeTwo.exitCode().has_value());
    assert(processWithExitCodeTwo.exitCode().value() == 2);
  }
#else
#warning <sys/types.h> or <sys/wait.h> is not found => \
    cu0::Process::wait() will not be checked
#warning <sys/types.h> or <sys/wait.h> is not found => \
    cu0::Process::waitCautious() will not be checked
#endif
#else
#warning __unix__ is not defined => \
    cu0::Process::wait() will not be checked
#warning __unix__ is not defined => \
    cu0::Process::waitCautious() will not be checked
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  assert(!thisProcess.exitCode().has_value());
#if __has_include(<unistd.h>)
  createdProcess.wait();
  assert(createdProcess.exitCode().value() == ENOENT);
  createdWithArgumentsProcess.wait();
  assert(createdWithArgumentsProcess.exitCode().value() == ENOENT);
  createdWithEnvironmentProcess.wait();
  assert(createdWithEnvironmentProcess.exitCode().value() == ENOENT);
  createdWithArgumentsAndEnvironmentProcess.wait();
  assert(
      createdWithArgumentsAndEnvironmentProcess.exitCode().value() == ENOENT
  );

  const auto executableWithExitCodeZero = cu0::Executable{
    .binary = argv[0],
    .arguments = {"0"},
  };
  auto createdWithExitCodeZero =
      cu0::Process::create(executableWithExitCodeZero);
  assert(std::holds_alternative<cu0::Process>(createdWithExitCodeZero));
  auto& processWithExitCodeZero =
      std::get<cu0::Process>(createdWithExitCodeZero);
  processWithExitCodeZero.wait();
  assert(processWithExitCodeZero.exitCode().value() == 0);

  const auto executableWithExitCodeOne = cu0::Executable{
    .binary = argv[0],
    .arguments = {"1"},
  };
  auto createdWithExitCodeOne =
      cu0::Process::create(executableWithExitCodeOne);
  assert(std::holds_alternative<cu0::Process>(createdWithExitCodeOne));
  auto& processWithExitCodeOne =
      std::get<cu0::Process>(createdWithExitCodeOne);
  processWithExitCodeOne.wait();
  assert(processWithExitCodeOne.exitCode().value() == 1);
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

#ifdef __unix__
#if __has_include(<unistd.h>)
  struct ProcessWriteIntoCheck : public cu0::Process {
    using cu0::Process::writeInto;
    ProcessWriteIntoCheck(cu0::Process&& process)
      : cu0::Process{std::move(process)} {}
    constexpr const int& stdinPipe() const {
      return this->stdinPipe_;
    }
  };
  {
    const auto executableWithExitCode64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto createdWithExitCode64 =
        cu0::Process::create(executableWithExitCode64);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode64));
    auto processWriteIntoCheck = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(createdWithExitCode64))
    };
    const auto& pipe = processWriteIntoCheck.stdinPipe();
    const auto [errorCode, bytesWritten] =
        ProcessWriteIntoCheck::writeInto<2>(pipe, "333\r\n");
    assert(errorCode == cu0::Process::WriteError::NO_ERROR);
    assert(bytesWritten == 5);
    processWriteIntoCheck.wait();
    assert(processWriteIntoCheck.exitCode().value() == 64);
    assert(processWriteIntoCheck.stdout() == "333");
    assert(processWriteIntoCheck.stderr() == "333333");
  }
  {
    const auto executableWithExitCode64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto createdWithExitCode64 =
        cu0::Process::create(executableWithExitCode64);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode64));
    auto processWriteIntoCheck = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(createdWithExitCode64))
    };
    const auto& pipe = processWriteIntoCheck.stdinPipe();
    const auto [errorCode, bytesWritten] =
        ProcessWriteIntoCheck::writeInto<3>(pipe, "333\r\n");
    assert(errorCode == cu0::Process::WriteError::NO_ERROR);
    assert(bytesWritten == 5);
    processWriteIntoCheck.wait();
    assert(processWriteIntoCheck.exitCode().value() == 64);
    assert(processWriteIntoCheck.stdout() == "333");
    assert(processWriteIntoCheck.stderr() == "333333");
  }
  {
    const auto executableWithExitCode64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto createdWithExitCode64 =
        cu0::Process::create(executableWithExitCode64);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode64));
    auto processWriteIntoCheck = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(createdWithExitCode64))
    };
    const auto& pipe = processWriteIntoCheck.stdinPipe();
    const auto [errorCode, bytesWritten] =
        ProcessWriteIntoCheck::writeInto<4>(pipe, "333\r\n");
    assert(errorCode == cu0::Process::WriteError::NO_ERROR);
    assert(bytesWritten == 5);
    processWriteIntoCheck.wait();
    assert(processWriteIntoCheck.exitCode().value() == 64);
    assert(processWriteIntoCheck.stdout() == "333");
    assert(processWriteIntoCheck.stderr() == "333333");
  }
  {
    const auto executableWithExitCode64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto createdWithExitCode64 =
        cu0::Process::create(executableWithExitCode64);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode64));
    auto processWriteIntoCheck = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(createdWithExitCode64))
    };
    const auto& pipe = processWriteIntoCheck.stdinPipe();
    const auto [errorCode, bytesWritten] =
        ProcessWriteIntoCheck::writeInto<1024>(pipe, "333\r\n");
    assert(errorCode == cu0::Process::WriteError::NO_ERROR);
    assert(bytesWritten == 5);
    processWriteIntoCheck.wait();
    assert(processWriteIntoCheck.exitCode().value() == 64);
    assert(processWriteIntoCheck.stdout() == "333");
    assert(processWriteIntoCheck.stderr() == "333333");
  }
  {
    const auto executableWithExitCode64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto createdWithExitCode64 =
        cu0::Process::create(executableWithExitCode64);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode64));
    auto processWriteIntoCheck = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(createdWithExitCode64))
    };
    const auto& pipe = processWriteIntoCheck.stdinPipe();
    const auto [errorCode, bytesWritten] =
        ProcessWriteIntoCheck::writeInto<8192>(pipe, "333\r\n");
    assert(errorCode == cu0::Process::WriteError::NO_ERROR);
    assert(bytesWritten == 5);
    processWriteIntoCheck.wait();
    assert(processWriteIntoCheck.exitCode().value() == 64);
    assert(processWriteIntoCheck.stdout() == "333");
    assert(processWriteIntoCheck.stderr() == "333333");
  }
  {
    const auto executableWithExitCode64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto createdWithExitCode64 =
        cu0::Process::create(executableWithExitCode64);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode64));
    auto& processWithExitCode64 =
        std::get<cu0::Process>(createdWithExitCode64);
    const auto [errorCode, bytesWritten] =
        processWithExitCode64.stdin("64\r\n");
    assert(errorCode == cu0::Process::WriteError::NO_ERROR);
    assert(bytesWritten == 4);
    processWithExitCode64.wait();
    assert(processWithExitCode64.exitCode().value() == 64);
    assert(processWithExitCode64.stdout() == "64");
    assert(processWithExitCode64.stderr() == "6464");
  }
#else
#warning <unistd.h> is not found => \
    cu0::Process::stdin() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::stdin() will not be checked
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
  std::cout << thisProcess.pid();
  std::cout.flush();
  assert(thisProcess.stdout().empty());
  std::cerr << thisProcess.pid() << thisProcess.pid();
  std::cerr.flush();
  assert(thisProcess.stderr().empty());

  assert(processWithExitCodeZero.stdout() == "0");
  assert(processWithExitCodeZero.stderr() == "00");
  assert(processWithExitCodeOne.stdout() == "1");
  assert(processWithExitCodeOne.stderr() == "11");

  struct ProcessReadFromCheck : public cu0::Process {
    using cu0::Process::readFrom;
    ProcessReadFromCheck(cu0::Process&& process)
      : cu0::Process{std::move(process)} {}
    constexpr const int& stdoutPipe() const {
      return this->stdoutPipe_;
    }
  };
  {
    const auto executableWithExitCode255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto createdWithExitCode255 =
        cu0::Process::create(executableWithExitCode255);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode255));
    auto& processWithExitCode255 =
        std::get<cu0::Process>(createdWithExitCode255);
    processWithExitCode255.wait();
    assert(processWithExitCode255.exitCode().value() == 255);
    const auto& processReadFromCheck =
        ProcessReadFromCheck{std::move(processWithExitCode255)};
    const auto& pipe = processReadFromCheck.stdoutPipe();
    assert(ProcessReadFromCheck::readFrom<2>(pipe) == "255");
  }
  {
    const auto executableWithExitCode255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto createdWithExitCode255 =
        cu0::Process::create(executableWithExitCode255);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode255));
    auto& processWithExitCode255 =
        std::get<cu0::Process>(createdWithExitCode255);
    processWithExitCode255.wait();
    assert(processWithExitCode255.exitCode().value() == 255);
    const auto& processReadFromCheck =
        ProcessReadFromCheck{std::move(processWithExitCode255)};;
    const auto& pipe = processReadFromCheck.stdoutPipe();
    assert(ProcessReadFromCheck::readFrom<3>(pipe) == "255");
  }
  {
    const auto executableWithExitCode255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto createdWithExitCode255 =
        cu0::Process::create(executableWithExitCode255);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode255));
    auto& processWithExitCode255 =
        std::get<cu0::Process>(createdWithExitCode255);
    processWithExitCode255.wait();
    assert(processWithExitCode255.exitCode().value() == 255);
    const auto& processReadFromCheck =
        ProcessReadFromCheck{std::move(processWithExitCode255)};
    const auto& pipe = processReadFromCheck.stdoutPipe();
    assert(ProcessReadFromCheck::readFrom<4>(pipe) == "255");
  }
  {
    const auto executableWithExitCode255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto createdWithExitCode255 =
        cu0::Process::create(executableWithExitCode255);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode255));
    auto& processWithExitCode255 =
        std::get<cu0::Process>(createdWithExitCode255);
    processWithExitCode255.wait();
    assert(processWithExitCode255.exitCode().value() == 255);
    const auto& processReadFromCheck =
        ProcessReadFromCheck{std::move(processWithExitCode255)};
    const auto& pipe = processReadFromCheck.stdoutPipe();
    assert(ProcessReadFromCheck::readFrom<1024>(pipe) == "255");
  }
  {
    const auto executableWithExitCode255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto createdWithExitCode255 =
        cu0::Process::create(executableWithExitCode255);
    assert(std::holds_alternative<cu0::Process>(createdWithExitCode255));
    auto& processWithExitCode255 =
        std::get<cu0::Process>(createdWithExitCode255);
    processWithExitCode255.wait();
    assert(processWithExitCode255.exitCode().value() == 255);
    const auto& processReadFromCheck =
        ProcessReadFromCheck{std::move(processWithExitCode255)};
    const auto& pipe = processReadFromCheck.stdoutPipe();
    assert(ProcessReadFromCheck::readFrom<8192>(pipe) == "255");
  }
#else
#warning <unistd.h> is not found => \
    cu0::Process::stdout() will not be checked
#warning <unistd.h> is not found => \
    cu0::Process::stderr() will not be checked
#endif
#else
#warning __unix__ is not defined => cu0::Process::stdout() will not be checked
#warning __unix__ is not defined => cu0::Process::stderr() will not be checked
#endif

#ifdef __unix__
#if __has_include(<signal.h>) && __has_include(<sys/types.h>) && \
    __has_include(<sys/wait.h>)
  {
    const auto executableWithSleep = cu0::Executable{
      .binary = argv[0],
      .arguments = {"128"},
    };
    const auto start = std::chrono::high_resolution_clock::now();
    auto createdWithSleep = cu0::Process::create(executableWithSleep);
    assert(std::holds_alternative<cu0::Process>(createdWithSleep));
    auto& processWithSleep =
        std::get<cu0::Process>(createdWithSleep);
    assert(!processWithSleep.exitCode().has_value());
    processWithSleep.signal(SIGTERM);
    processWithSleep.wait();
    const auto end = std::chrono::high_resolution_clock::now();
    assert(!processWithSleep.exitCode().has_value());
    assert(processWithSleep.terminationCode().has_value());
    assert(processWithSleep.terminationCode().value() == SIGTERM);
    assert(
        std::chrono::duration_cast<std::chrono::seconds>(end - start) <
            //! divide by 2 to minimize calculation errors
            std::chrono::seconds{SLEEP_DURATION} / 2
    );
  }
  {
    const auto executableWithSleep = cu0::Executable{
      .binary = argv[0],
      .arguments = {"128"},
    };
    const auto start = std::chrono::high_resolution_clock::now();
    auto createdWithSleep = cu0::Process::create(executableWithSleep);
    assert(std::holds_alternative<cu0::Process>(createdWithSleep));
    auto& processWithSleep =
        std::get<cu0::Process>(createdWithSleep);
    assert(!processWithSleep.exitCode().has_value());
    processWithSleep.signal(SIGKILL);
    processWithSleep.wait();
    const auto end = std::chrono::high_resolution_clock::now();
    assert(!processWithSleep.exitCode().has_value());
    assert(processWithSleep.terminationCode().has_value());
    assert(processWithSleep.terminationCode().value() == SIGKILL);
    assert(
        std::chrono::duration_cast<std::chrono::seconds>(end - start) <
            //! divide by 2 to minimize calculation errors
            std::chrono::seconds{SLEEP_DURATION} / 2
    );
  }
#else
#warning <signal.h> or <sys/types.h> or <sys/wait.h> is not found => \
    cu0::Process::signal() and cu0::Process::terminationCode() will not be \
    checked
#endif
#else
#warning __unix__ is not defined => \
    cu0::Process::signal() and cu0::Process::terminationCode() will not be \
    checked
#endif

  return 0;
}
