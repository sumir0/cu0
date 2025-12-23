#include <cu0/proc/process.hh>
#include <cassert>
#include <array>
#include <atomic>
#include <iostream>
#include <thread>

int main(int argc, char** argv) {

#if __has_include(<unistd.h>)
  const auto this_process = cu0::Process::current();
  assert(this_process.pid() == static_cast<unsigned>(getpid()));
#else
#warning <unistd.h> is not found => cu0::Process::current() will not be checked
#endif

  struct ProcessCheck : public cu0::Process {
    constexpr ProcessCheck() = default;
  };
  const auto some_process = ProcessCheck{};
  assert(some_process.pid() == 0);

#if __has_include(<unistd.h>)
  const auto executable = cu0::Executable{}; //! empty executable
  auto created = cu0::Process::create(executable);
  assert(std::holds_alternative<cu0::Process>(created));
  auto& created_process = std::get<cu0::Process>(created);
  assert(created_process.pid() != 0);
  assert(static_cast<bool>(created_process.stdin_pipe()));
  assert(static_cast<bool>(created_process.stdout_pipe()));
  assert(static_cast<bool>(created_process.stderr_pipe()));

  const auto executable_with_arguments = cu0::Executable{
    .arguments = { "arg1", "arg2", "arg3", },
  };
  auto created_with_arguments =
      cu0::Process::create(executable_with_arguments);
  assert(std::holds_alternative<cu0::Process>(created_with_arguments));
  auto& created_with_arguments_process =
      std::get<cu0::Process>(created_with_arguments);
  assert(created_with_arguments_process.pid() != 0);
  assert(static_cast<bool>(created_with_arguments_process.stdin_pipe()));
  assert(static_cast<bool>(created_with_arguments_process.stdout_pipe()));
  assert(static_cast<bool>(created_with_arguments_process.stderr_pipe()));

  const auto executable_with_environment = cu0::Executable{
    .environment = { { "k1", "v1", }, { "k2", "v2", }, { "k3", "v3", } },
  };
  auto created_with_environment =
      cu0::Process::create(executable_with_environment);
  assert(std::holds_alternative<cu0::Process>(created_with_environment));
  auto& created_with_environment_process =
      std::get<cu0::Process>(created_with_environment);
  assert(created_with_environment_process.pid() != 0);
  assert(static_cast<bool>(created_with_environment_process.stdin_pipe()));
  assert(static_cast<bool>(created_with_environment_process.stdout_pipe()));
  assert(static_cast<bool>(created_with_environment_process.stderr_pipe()));

  const auto executable_with_arguments_and_environment = cu0::Executable{
    .arguments = { "arg1", "arg2", "arg3", },
    .environment = { { "k1", "v1", }, { "k2", "v2", }, { "k3", "v3", } },
  };
  auto created_with_arguments_and_environment =
      cu0::Process::create(executable_with_arguments_and_environment);
  assert(std::holds_alternative<cu0::Process>(
      created_with_arguments_and_environment
  ));
  auto& created_with_arguments_and_environment_process =
      std::get<cu0::Process>(created_with_arguments_and_environment);
  assert(created_with_arguments_and_environment_process.pid() != 0);
  assert(static_cast<bool>(
      created_with_arguments_and_environment_process.stdin_pipe()
  ));
  assert(static_cast<bool>(
      created_with_arguments_and_environment_process.stdout_pipe()
  ));
  assert(static_cast<bool>(
      created_with_arguments_and_environment_process.stderr_pipe()
  ));
#else
#warning <unistd.h> is not found => cu0::Process::create() will not be checked
#endif

#if __has_include(<unistd.h>)
  const auto created_pipeless = cu0::Process::create_pipeless(executable);
  assert(std::holds_alternative<cu0::Process>(created_pipeless));
  const auto& created_process_pipeless =
      std::get<cu0::Process>(created_pipeless);
  assert(created_process_pipeless.pid() != 0);
  assert(!static_cast<bool>(created_process_pipeless.stdin_pipe()));
  assert(!static_cast<bool>(created_process_pipeless.stdout_pipe()));
  assert(!static_cast<bool>(created_process_pipeless.stderr_pipe()));

  const auto created_with_arguments_pipeless =
      cu0::Process::create_pipeless(executable_with_arguments);
  assert(std::holds_alternative<cu0::Process>(created_with_arguments_pipeless));
  const auto& created_with_arguments_process_pipeless =
      std::get<cu0::Process>(created_with_arguments_pipeless);
  assert(created_with_arguments_process_pipeless.pid() != 0);
  assert(!static_cast<bool>(
      created_with_arguments_process_pipeless.stdin_pipe()
  ));
  assert(!static_cast<bool>(
      created_with_arguments_process_pipeless.stdout_pipe()
  ));
  assert(!static_cast<bool>(
      created_with_arguments_process_pipeless.stderr_pipe()
  ));

  const auto created_with_environment_pipeless =
      cu0::Process::create_pipeless(executable_with_environment);
  assert(std::holds_alternative<cu0::Process>(
      created_with_environment_pipeless
  ));
  const auto& created_with_environment_process_pipeless =
      std::get<cu0::Process>(created_with_environment_pipeless);
  assert(created_with_environment_process_pipeless.pid() != 0);
  assert(!static_cast<bool>(
      created_with_environment_process_pipeless.stdin_pipe()
  ));
  assert(!static_cast<bool>(
      created_with_environment_process_pipeless.stdout_pipe()
  ));
  assert(!static_cast<bool>(
      created_with_environment_process_pipeless.stderr_pipe()
  ));

  const auto created_with_arguments_and_environment_pipeless =
      cu0::Process::create_pipeless(executable_with_arguments_and_environment);
  assert(std::holds_alternative<cu0::Process>(
      created_with_arguments_and_environment_pipeless
  ));
  const auto& created_with_arguments_and_environment_process_pipeless =
      std::get<cu0::Process>(created_with_arguments_and_environment_pipeless);
  assert(created_with_arguments_and_environment_process_pipeless.pid() != 0);
  assert(!static_cast<bool>(
      created_with_arguments_and_environment_process_pipeless.stdin_pipe()
  ));
  assert(!static_cast<bool>(
      created_with_arguments_and_environment_process_pipeless.stdout_pipe()
  ));
  assert(!static_cast<bool>(
      created_with_arguments_and_environment_process_pipeless.stderr_pipe()
  ));
#else
#warning <unistd.h> is not found => \
cu0::Process::create_pipeless() will not be checked
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

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  {
    const auto executable_with_exit_code_two = cu0::Executable{
      .binary = argv[0],
      .arguments = {"2"},
    };
    auto created_with_exit_code_two =
        cu0::Process::create(executable_with_exit_code_two);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_two));
    auto& process_with_exit_code_two =
        std::get<cu0::Process>(created_with_exit_code_two);
    std::atomic<bool> wait_fail = true;
    auto wait_thread = std::thread([&process_with_exit_code_two, &wait_fail](){
      process_with_exit_code_two.wait();
      wait_fail = false;
    });
    const auto wait_thread_start_time =
        std::chrono::high_resolution_clock::now();
    while (
        wait_fail &&
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::high_resolution_clock::now() - wait_thread_start_time
        ) < std::chrono::seconds{32}
    ) {
      //! do nothing, wait
    }
    if (wait_thread.joinable()) {
      wait_thread.join();
    }
    assert(!wait_fail);
    process_with_exit_code_two.wait();
    assert(process_with_exit_code_two.exit_code().has_value());
    assert(process_with_exit_code_two.exit_code().value() == 2);
    process_with_exit_code_two.wait();
    assert(process_with_exit_code_two.exit_code().has_value());
    assert(process_with_exit_code_two.exit_code().value() == 2);
    const auto variant = process_with_exit_code_two.wait_cautious();
    assert(std::holds_alternative<cu0::Process::WaitError>(variant));
    assert(
        std::get<cu0::Process::WaitError>(variant) ==
            cu0::Process::WaitError::CHILD
    );
    assert(process_with_exit_code_two.exit_code().has_value());
    assert(process_with_exit_code_two.exit_code().value() == 2);
  }
  {
    const auto executable_with_exit_code_two = cu0::Executable{
      .binary = argv[0],
      .arguments = {"2"},
    };
    auto created_with_exit_code_two =
        cu0::Process::create(executable_with_exit_code_two);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_two));
    std::atomic<bool> wait_fail = true;
    auto& process_with_exit_code_two =
        std::get<cu0::Process>(created_with_exit_code_two);
    auto wait_thread = std::thread([&process_with_exit_code_two, &wait_fail](){
      const auto variant = process_with_exit_code_two.wait_cautious();
      assert(std::holds_alternative<std::monostate>(variant));
      wait_fail = false;
    });
    const auto wait_thread_start_time =
        std::chrono::high_resolution_clock::now();
    while (
        wait_fail &&
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::high_resolution_clock::now() - wait_thread_start_time
        ) < std::chrono::seconds{32}
    ) {
      //! do nothing, wait
    }
    if (wait_thread.joinable()) {
      wait_thread.join();
    }
    assert(!wait_fail);
    const auto variant = process_with_exit_code_two.wait_cautious();
    assert(std::holds_alternative<cu0::Process::WaitError>(variant));
    assert(
        std::get<cu0::Process::WaitError>(variant) ==
            cu0::Process::WaitError::CHILD
    );
    assert(process_with_exit_code_two.exit_code().has_value());
    assert(process_with_exit_code_two.exit_code().value() == 2);
    const auto variant_other = process_with_exit_code_two.wait_cautious();
    assert(std::holds_alternative<cu0::Process::WaitError>(variant_other));
    assert(
        std::get<cu0::Process::WaitError>(variant_other) ==
            cu0::Process::WaitError::CHILD
    );
    assert(process_with_exit_code_two.exit_code().has_value());
    assert(process_with_exit_code_two.exit_code().value() == 2);
    process_with_exit_code_two.wait();
    assert(process_with_exit_code_two.exit_code().has_value());
    assert(process_with_exit_code_two.exit_code().value() == 2);
  }
#else
#warning <sys/types.h> or <sys/wait.h> is not found => \
cu0::Process::wait() will not be checked
#warning <sys/types.h> or <sys/wait.h> is not found => \
cu0::Process::wait_cautious() will not be checked
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  assert(!this_process.exit_code().has_value());
#if __has_include(<unistd.h>)
  created_process.wait();
  assert(created_process.exit_code().value() == ENOENT);
  created_with_arguments_process.wait();
  assert(created_with_arguments_process.exit_code().value() == ENOENT);
  created_with_environment_process.wait();
  assert(created_with_environment_process.exit_code().value() == ENOENT);
  created_with_arguments_and_environment_process.wait();
  assert(
      created_with_arguments_and_environment_process.exit_code().value() == ENOENT
  );

  const auto executable_with_exit_code_zero = cu0::Executable{
    .binary = argv[0],
    .arguments = {"0"},
  };
  auto created_with_exit_code_zero =
      cu0::Process::create(executable_with_exit_code_zero);
  assert(std::holds_alternative<cu0::Process>(created_with_exit_code_zero));
  auto& process_with_exit_code_zero =
      std::get<cu0::Process>(created_with_exit_code_zero);
  process_with_exit_code_zero.wait();
  assert(process_with_exit_code_zero.exit_code().value() == 0);

  const auto executable_with_exit_code_one = cu0::Executable{
    .binary = argv[0],
    .arguments = {"1"},
  };
  auto created_with_exit_code_one =
      cu0::Process::create(executable_with_exit_code_one);
  assert(std::holds_alternative<cu0::Process>(created_with_exit_code_one));
  auto& process_with_exit_code_one =
      std::get<cu0::Process>(created_with_exit_code_one);
  process_with_exit_code_one.wait();
  assert(process_with_exit_code_one.exit_code().value() == 1);
#else
#warning <unistd.h> is not found => \
    cu0::Process::exit_code() will not be checked for created processes
#endif
#else
#warning <sys/types.h> or <sys/wait.h> is not found => \
cu0::Process::exit_code() will not be checked
#endif

#if __has_include(<unistd.h>)
  struct ProcessWriteIntoCheck : public cu0::Process {
    using cu0::Process::write_into;
    ProcessWriteIntoCheck(cu0::Process&& process)
      : cu0::Process{std::move(process)} {}
    constexpr const int& stdin_pipe() const {
      return this->stdin_pipe_;
    }
  };
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    ProcessWriteIntoCheck::write_into<2, void>(pipe, "333\r\n");
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    assert(process_write_into_check.stdout() == "333");
    assert(process_write_into_check.stderr() == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    ProcessWriteIntoCheck::write_into<3, void>(pipe, "333\r\n");
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    assert(process_write_into_check.stdout() == "333");
    assert(process_write_into_check.stderr() == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    ProcessWriteIntoCheck::write_into<4, void>(pipe, "333\r\n");
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    assert(process_write_into_check.stdout() == "333");
    assert(process_write_into_check.stderr() == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    ProcessWriteIntoCheck::write_into<1024, void>(pipe, "333\r\n");
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    assert(process_write_into_check.stdout() == "333");
    assert(process_write_into_check.stderr() == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    ProcessWriteIntoCheck::write_into<8192, void>(pipe, "333\r\n");
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    assert(process_write_into_check.stdout() == "333");
    assert(process_write_into_check.stderr() == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto& process_with_exit_code_64 =
        std::get<cu0::Process>(created_with_exit_code_64);
    process_with_exit_code_64.stdin("64\r\n");
    process_with_exit_code_64.wait();
    assert(process_with_exit_code_64.exit_code().value() == 64);
    assert(process_with_exit_code_64.stdout() == "64");
    assert(process_with_exit_code_64.stderr() == "6464");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    const auto [write_result, bytes_written] =
        ProcessWriteIntoCheck::write_into<
            2,
            std::tuple<
                std::variant<std::monostate, typename cu0::Process::WriteError>,
                std::size_t
            >
        >(pipe, "333\r\n");
    assert(std::holds_alternative<std::monostate>(write_result));
    assert(bytes_written == 5);
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    const auto [stdout_result, out_str] =
        process_write_into_check.stdout_cautious();
    assert(std::holds_alternative<std::monostate>(stdout_result));
    assert(out_str == "333");
    const auto [stderr_result, err_str] =
        process_write_into_check.stderr_cautious();
    assert(std::holds_alternative<std::monostate>(stderr_result));
    assert(err_str == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    const auto [write_result, bytes_written] =
        ProcessWriteIntoCheck::write_into<
            3,
            std::tuple<
                std::variant<std::monostate, typename cu0::Process::WriteError>,
                std::size_t
            >
        >(pipe, "333\r\n");
    assert(std::holds_alternative<std::monostate>(write_result));
    assert(bytes_written == 5);
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    const auto [stdout_result, out_str] =
        process_write_into_check.stdout_cautious();
    assert(std::holds_alternative<std::monostate>(stdout_result));
    assert(out_str == "333");
    const auto [stderr_result, err_str] =
        process_write_into_check.stderr_cautious();
    assert(std::holds_alternative<std::monostate>(stderr_result));
    assert(err_str == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    const auto [write_result, bytes_written] =
        ProcessWriteIntoCheck::write_into<
            4,
            std::tuple<
                std::variant<std::monostate, typename cu0::Process::WriteError>,
                std::size_t
            >
        >(pipe, "333\r\n");
    assert(std::holds_alternative<std::monostate>(write_result));
    assert(bytes_written == 5);
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    const auto [stdout_result, out_str] =
        process_write_into_check.stdout_cautious();
    assert(std::holds_alternative<std::monostate>(stdout_result));
    assert(out_str == "333");
    const auto [stderr_result, err_str] =
        process_write_into_check.stderr_cautious();
    assert(std::holds_alternative<std::monostate>(stderr_result));
    assert(err_str == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    const auto [write_result, bytes_written] =
        ProcessWriteIntoCheck::write_into<
            1024,
            std::tuple<
                std::variant<std::monostate, typename cu0::Process::WriteError>,
                std::size_t
            >
        >(pipe, "333\r\n");
    assert(std::holds_alternative<std::monostate>(write_result));
    assert(bytes_written == 5);
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    const auto [stdout_result, out_str] =
        process_write_into_check.stdout_cautious();
    assert(std::holds_alternative<std::monostate>(stdout_result));
    assert(out_str == "333");
    const auto [stderr_result, err_str] =
        process_write_into_check.stderr_cautious();
    assert(std::holds_alternative<std::monostate>(stderr_result));
    assert(err_str == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto process_write_into_check = ProcessWriteIntoCheck{
      std::get<cu0::Process>(std::move(created_with_exit_code_64))
    };
    const auto& pipe = process_write_into_check.stdin_pipe();
    const auto [write_result, bytes_written] =
        ProcessWriteIntoCheck::write_into<
            8192,
            std::tuple<
                std::variant<std::monostate, typename cu0::Process::WriteError>,
                std::size_t
            >
        >(pipe, "333\r\n");
    assert(std::holds_alternative<std::monostate>(write_result));
    assert(bytes_written == 5);
    process_write_into_check.wait();
    assert(process_write_into_check.exit_code().value() == 64);
    const auto [stdout_result, out_str] =
        process_write_into_check.stdout_cautious();
    assert(std::holds_alternative<std::monostate>(stdout_result));
    assert(out_str == "333");
    const auto [stderr_result, err_str] =
        process_write_into_check.stderr_cautious();
    assert(std::holds_alternative<std::monostate>(stderr_result));
    assert(err_str == "333333");
  }
  {
    const auto executable_with_exit_code_64 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"64"},
    };
    auto created_with_exit_code_64 =
        cu0::Process::create(executable_with_exit_code_64);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_64));
    auto& process_with_exit_code_64 =
        std::get<cu0::Process>(created_with_exit_code_64);
    const auto [result, bytes_written] =
        process_with_exit_code_64.stdin_cautious("64\r\n");
    assert(std::holds_alternative<std::monostate>(result));
    assert(bytes_written == 4);
    process_with_exit_code_64.wait();
    assert(process_with_exit_code_64.exit_code().value() == 64);
    const auto [stdout_result, out_str] =
        process_with_exit_code_64.stdout_cautious();
    assert(std::holds_alternative<std::monostate>(stdout_result));
    assert(out_str == "64");
    const auto [stderr_result, err_str] =
        process_with_exit_code_64.stderr_cautious();
    assert(std::holds_alternative<std::monostate>(stderr_result));
    assert(err_str == "6464");
  }
#else
#warning <unistd.h> is not found => \
cu0::Process::stdin() will not be checked
#warning <unistd.h> is not found => \
cu0::Process::stdin_cautious() will not be checked
#endif

#if __has_include(<unistd.h>)
  std::cout << this_process.pid();
  std::cout.flush();
  assert(this_process.stdout().empty());
  const auto [stdout_result, out_str] = this_process.stdout_cautious();
  assert(out_str.empty());
  std::cerr << this_process.pid() << this_process.pid();
  std::cerr.flush();
  assert(this_process.stderr().empty());
  const auto [stderr_result, err_str] = this_process.stderr_cautious();
  assert(err_str.empty());

  const auto [zero_stdout_result, zero_out_str] =
      process_with_exit_code_zero.stdout_cautious();
  assert(std::holds_alternative<std::monostate>(zero_stdout_result));
  assert(zero_out_str == "0");
  assert(process_with_exit_code_zero.stderr() == "00");
  assert(process_with_exit_code_one.stdout() == "1");
  const auto [one_stderr_result, one_err_str] =
      process_with_exit_code_one.stderr_cautious();
  assert(std::holds_alternative<std::monostate>(one_stderr_result));
  assert(one_err_str == "11");

  struct ProcessReadFromCheck : public cu0::Process {
    using cu0::Process::read_from;
    ProcessReadFromCheck(cu0::Process&& process)
      : cu0::Process{std::move(process)} {}
    constexpr const int& stdout_pipe() const {
      return this->stdout_pipe_;
    }
  };
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto read = ProcessReadFromCheck::read_from<2, std::string>(pipe);
    assert(read == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};;
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto read = ProcessReadFromCheck::read_from<3, std::string>(pipe);
    assert(read == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto read = ProcessReadFromCheck::read_from<4, std::string>(pipe);
    assert(read == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto read = ProcessReadFromCheck::read_from<1024, std::string>(pipe);
    assert(read == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto read = ProcessReadFromCheck::read_from<8192, std::string>(pipe);
    assert(read == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto [variant, str] = ProcessReadFromCheck::read_from<
        2,
        std::tuple<
            std::variant<std::monostate, typename cu0::Process::ReadError>,
            std::string
        >
    >(pipe);
    assert(std::holds_alternative<std::monostate>(variant));
    assert(str == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};;
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto [variant, str] = ProcessReadFromCheck::read_from<
        3,
        std::tuple<
            std::variant<std::monostate, typename cu0::Process::ReadError>,
            std::string
        >
    >(pipe);
    assert(std::holds_alternative<std::monostate>(variant));
    assert(str == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto [variant, str] = ProcessReadFromCheck::read_from<
        4,
        std::tuple<
            std::variant<std::monostate, typename cu0::Process::ReadError>,
            std::string
        >
    >(pipe);
    assert(std::holds_alternative<std::monostate>(variant));
    assert(str == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto [variant, str] = ProcessReadFromCheck::read_from<
        1024,
        std::tuple<
            std::variant<std::monostate, typename cu0::Process::ReadError>,
            std::string
        >
    >(pipe);
    assert(std::holds_alternative<std::monostate>(variant));
    assert(str == "255");
  }
  {
    const auto executable_with_exit_code_255 = cu0::Executable{
      .binary = argv[0],
      .arguments = {"255"},
    };
    auto created_with_exit_code_255 =
        cu0::Process::create(executable_with_exit_code_255);
    assert(std::holds_alternative<cu0::Process>(created_with_exit_code_255));
    auto& process_with_exit_code_255 =
        std::get<cu0::Process>(created_with_exit_code_255);
    process_with_exit_code_255.wait();
    assert(process_with_exit_code_255.exit_code().value() == 255);
    const auto& process_read_from_check =
        ProcessReadFromCheck{std::move(process_with_exit_code_255)};
    const auto& pipe = process_read_from_check.stdout_pipe();
    const auto [variant, str] = ProcessReadFromCheck::read_from<
        8192,
        std::tuple<
            std::variant<std::monostate, typename cu0::Process::ReadError>,
            std::string
        >
    >(pipe);
    assert(std::holds_alternative<std::monostate>(variant));
    assert(str == "255");
  }
#else
#warning <unistd.h> is not found => \
cu0::Process::stdout() will not be checked
#warning <unistd.h> is not found => \
cu0::Process::stdout_cautious() will not be checked
#warning <unistd.h> is not found => \
cu0::Process::stderr() will not be checked
#warning <unistd.h> is not found => \
cu0::Process::stderr_cautious() will not be checked
#endif

#if __has_include(<signal.h>) && __has_include(<sys/types.h>) && \
__has_include(<sys/wait.h>)
  {
    const auto executable_with_sleep = cu0::Executable{
      .binary = argv[0],
      .arguments = {"128"},
    };
    const auto start = std::chrono::high_resolution_clock::now();
    auto created_with_sleep = cu0::Process::create(executable_with_sleep);
    assert(std::holds_alternative<cu0::Process>(created_with_sleep));
    auto& process_with_sleep =
        std::get<cu0::Process>(created_with_sleep);
    assert(!process_with_sleep.exit_code().has_value());
    process_with_sleep.signal(SIGTERM);
    process_with_sleep.wait();
    const auto end = std::chrono::high_resolution_clock::now();
    assert(!process_with_sleep.exit_code().has_value());
    assert(process_with_sleep.termination_code().has_value());
    assert(process_with_sleep.termination_code().value() == SIGTERM);
    assert(
        std::chrono::duration_cast<std::chrono::seconds>(end - start) <
            //! divide by 2 to minimize calculation errors
            std::chrono::seconds{SLEEP_DURATION} / 2
    );
  }
  {
    const auto executable_with_sleep = cu0::Executable{
      .binary = argv[0],
      .arguments = {"128"},
    };
    const auto start = std::chrono::high_resolution_clock::now();
    auto created_with_sleep = cu0::Process::create(executable_with_sleep);
    assert(std::holds_alternative<cu0::Process>(created_with_sleep));
    auto& process_with_sleep =
        std::get<cu0::Process>(created_with_sleep);
    assert(!process_with_sleep.exit_code().has_value());
    process_with_sleep.signal(SIGKILL);
    process_with_sleep.wait();
    const auto end = std::chrono::high_resolution_clock::now();
    assert(!process_with_sleep.exit_code().has_value());
    assert(process_with_sleep.termination_code().has_value());
    assert(process_with_sleep.termination_code().value() == SIGKILL);
    assert(
        std::chrono::duration_cast<std::chrono::seconds>(end - start) <
            //! divide by 2 to minimize calculation errors
            std::chrono::seconds{SLEEP_DURATION} / 2
    );
  }
  {
    const auto executable_with_sleep = cu0::Executable{
      .binary = argv[0],
      .arguments = {"128"},
    };
    const auto start = std::chrono::high_resolution_clock::now();
    auto created_with_sleep = cu0::Process::create(executable_with_sleep);
    assert(std::holds_alternative<cu0::Process>(created_with_sleep));
    auto& process_with_sleep =
        std::get<cu0::Process>(created_with_sleep);
    assert(!process_with_sleep.exit_code().has_value());
    const auto variant = process_with_sleep.signal_cautious(SIGTERM);
    assert(std::holds_alternative<std::monostate>(variant));
    process_with_sleep.wait();
    const auto end = std::chrono::high_resolution_clock::now();
    assert(!process_with_sleep.exit_code().has_value());
    assert(process_with_sleep.termination_code().has_value());
    assert(process_with_sleep.termination_code().value() == SIGTERM);
    assert(
        std::chrono::duration_cast<std::chrono::seconds>(end - start) <
            //! divide by 2 to minimize calculation errors
            std::chrono::seconds{SLEEP_DURATION} / 2
    );
  }
  {
    const auto executable_with_sleep = cu0::Executable{
      .binary = argv[0],
      .arguments = {"128"},
    };
    const auto start = std::chrono::high_resolution_clock::now();
    auto created_with_sleep = cu0::Process::create(executable_with_sleep);
    assert(std::holds_alternative<cu0::Process>(created_with_sleep));
    auto& process_with_sleep =
        std::get<cu0::Process>(created_with_sleep);
    assert(!process_with_sleep.exit_code().has_value());
    const auto variant = process_with_sleep.signal_cautious(SIGKILL);
    assert(std::holds_alternative<std::monostate>(variant));
    process_with_sleep.wait();
    const auto end = std::chrono::high_resolution_clock::now();
    assert(!process_with_sleep.exit_code().has_value());
    assert(process_with_sleep.termination_code().has_value());
    assert(process_with_sleep.termination_code().value() == SIGKILL);
    assert(
        std::chrono::duration_cast<std::chrono::seconds>(end - start) <
            //! divide by 2 to minimize calculation errors
            std::chrono::seconds{SLEEP_DURATION} / 2
    );
  }
#else
#warning <signal.h> or <sys/types.h> or <sys/wait.h> is not found => \
cu0::Process::signal() and cu0::Process::signal_cautious() and \
cu0::Process::termination_code() will not be checked
#endif

  {
    constexpr struct ProcessDataMembers {
      unsigned pid;
      int stdin_pipe;
      int stdout_pipe;
      int stderr_pipe;
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
      std::optional<int> exit_code;
      std::optional<int> termination_code;
      std::optional<int> stop_code;
#else
#warning <sys/types.h> or <sys/wait.h> was not found => \
cu0::Process::stop_code() will not be checked
#endif
    } process_data_members{
      .pid = 512,
      .stdin_pipe = 511,
      .stdout_pipe = 510,
      .stderr_pipe = 509,
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
      .exit_code = 508,
      .termination_code = 507,
      .stop_code = 506,
#endif
    };
    struct ProcessDataMembersSetter : public cu0::Process {
      constexpr ProcessDataMembersSetter(ProcessDataMembers data) {
        this->pid_ = std::move(data.pid);
        this->stdin_pipe_ = std::move(data.stdin_pipe);
        this->stdout_pipe_ = std::move(data.stdout_pipe);
        this->stderr_pipe_ = std::move(data.stderr_pipe);
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
        this->exit_code_ = std::move(data.exit_code);
        this->termination_code_ = std::move(data.termination_code);
        this->stop_code_ = std::move(data.stop_code);
#endif
      }
    };
    auto process_with_custom_data =
        ProcessDataMembersSetter{process_data_members}; //! parameter is copied
    auto other_process_with_custom_data =
        ProcessDataMembersSetter{process_data_members}; //! parameter is copied
    const auto moved_with_ctor = cu0::Process(std::move(
        process_with_custom_data
    ));
    assert(moved_with_ctor.pid() == process_data_members.pid);
    assert(moved_with_ctor.stdin_pipe() == process_data_members.stdin_pipe);
    assert(moved_with_ctor.stdout_pipe() == process_data_members.stdout_pipe);
    assert(moved_with_ctor.stderr_pipe() == process_data_members.stderr_pipe);
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
    assert(moved_with_ctor.exit_code() == process_data_members.exit_code);
    assert(
        moved_with_ctor.termination_code() ==
            process_data_members.termination_code
    );
    assert(moved_with_ctor.stop_code() == process_data_members.stop_code);
#endif
    const auto moved_with_operator = cu0::Process(std::move(
        other_process_with_custom_data
    ));
    assert(moved_with_operator.pid() == process_data_members.pid);
    assert(
        moved_with_operator.stdin_pipe() == process_data_members.stdin_pipe
    );
    assert(
        moved_with_operator.stdout_pipe() == process_data_members.stdout_pipe
    );
    assert(
        moved_with_operator.stderr_pipe() == process_data_members.stderr_pipe
    );
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
    assert(moved_with_operator.exit_code() == process_data_members.exit_code);
    assert(
        moved_with_operator.termination_code() ==
            process_data_members.termination_code
    );
    assert(moved_with_operator.stop_code() == process_data_members.stop_code);
#endif
  }

  return 0;
}
