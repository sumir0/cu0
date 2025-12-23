# cu0

cu0 is a header-only library providing common utilities for software engineering.

## Testing

| Compiler | Setting                                                    | Status       |
| :------- | :--------------------------------------------------------- | :----------: |
| GCC      | CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=ON  | Build passed |
| GCC      | CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=OFF | Build failed |
| Clang    | CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=ON  | Build passed |
| Clang    | CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=OFF | Build passed |

```console
[CC=<path-to-c-compiler>] [CXX=<path-to-cxx-compiler>] cmake \
  -S <path-to-local-repository> \
  -B <path-to-build-directory> \
  [-D<setting=value> ...]
cmake --build <path-to-build-directory>
cmake --build <path-to-build-directory> --target test
```

### CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT

CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=OFF to compile specialization declarations in structs. 

By default CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=ON.

Some compilers are known to have bugs. This setting allows the code which uses the library to be compiled by such compilers (at the time of writing this).

## Features

### cu0::Environment

#### Get a snapshot of the environment

`examples/example_cu0_environment.cc`
```c++
#include <cu0/env/environment.hh>
#include <iostream>

int main() {
  //! @note not supported on all platforms yet
  //! @note environment variable contains current environment's key-value data
  //! @note environment variable is not synchronized with the actual environment
  //!     i.e. environment variable is a snapshot of the actual environment
  const auto& environment = 
      cu0::Environment::as<std::map<std::string, std::string>>();
  //! iterate through the snapshot of the actual environment
  for (const auto& [key, value] : environment) {
    std::cout << "environment[" << key << "]" << "=" << value << '\n';
  }
}
```

### cu0::EnvironmentVariable

#### Get an environment variable value

`examples/example_cu0_environment_variable_cached.cc`
```c++
#include <cu0/env/environment_variable.hh>
#include <iostream>

int main() {
  //! @note an environment variable may not be set
  //! @note read value of the environment variable with the key 'KEY'
  const auto environment_variable = cu0::EnvironmentVariable::synced("KEY");
  //! @note if the environment variable is not set =>
  //!     an empty optional is returned
  //! @note cached contains the value of the environment variable at the time of
  //!     last sync with the environment @see EnvironmentVariable::sync
  const auto& cached = environment_variable.cached();
  std::cout << "Value of the environment variable 'KEY': " <<
      (cached.has_value() ? cached.value() : "<not-set>") << '\n';
}
```

#### Synchronise an environment variable value

`examples/example_cu0_environment_variable_sync.cc`
```c++
#include <cu0/env/environment_variable.hh>
#include <iostream>

int main() {
  //! @note environment variable may not be set
  auto environment_variable = cu0::EnvironmentVariable::unsynced("KEY");
  //! @note wait for external environment variable change and
  //!     sync value of the environment variable with the key 'KEY'
  const auto& synced = environment_variable.sync();
  std::cout << "Synced value of the environment variable 'KEY': " <<
      (synced.has_value() ? synced.value() : "<not-set>") << '\n';
}
```

#### Set an environment variable value

`examples/example_cu0_environment_variable_set.cc`
```c++
#include <cu0/env/environment_variable.hh>
#include <iostream>

int main() {
  //! @note not supported on all platforms yet
  //! @note create an environment variable without syncing with the environment
  //!     because we will set the value of the environment variable later
  //!     no syncing before that is needed
  auto environment_variable = cu0::EnvironmentVariable::unsynced("KEY");
  //! @note set the value of the environment variable
  //! @note automatically updates the environment with the value
  environment_variable.set("some_value");
  //! @note access cached value of the environment variable
  const auto& cached = environment_variable.cached();
  std::cout << "Value of the environment variable 'KEY': " <<
      (cached.has_value() ? cached.value() : "<not-set>") << '\n';
}
```

### cu0::Executable

#### Find an executable by a name and a directory

`examples/example_cu0_executable_find_by_name_and_directory.cc`
```c++
#include <cu0/proc/executable.hh>
#include <filesystem>
#include <iostream>

int main() {
  //! @note executable can be searched in a directory
  const auto executable = cu0::util::find_by(
      "some_executable_name", 
      std::filesystem::current_path()
  );
  //! @note if no executable is found then executable.binary is empty
  if (executable.binary.empty()) {
    std::cout << "Executable with the specified name was not found" << '\n';
  } else {
    //! @note if an executable is found then executable.binary is a path to it
    std::cout << "Path to the executable: " << executable.binary << '\n';
  }
}
```

#### Find an executable by a name 

##### Searches in the order:
 * the current directory,
 * the directories specified by the PATH environment variable.

`examples/example_cu0_executable_find_by_name.cc`
```c++
#include <cu0/proc/executable.hh>
#include <iostream>

int main() {
  //! @note executable can be searched in the current directory and
  //!     in the directories specified by the PATH environment variable
  const auto executable = cu0::util::find_by("some_executable_name");
  //! @note if no executable is found then executable.binary is empty
  if (executable.binary.empty()) {
    std::cout << "Executable with the specified name was not found" << '\n';
  } else {
    //! @note if an executable is found then executable.binary is a path to it
    std::cout << "Path to the executable: " << executable.binary << '\n';
  }
}
```

### cu0::Process

#### Create a process

`examples/example_cu0_process_create.cc`
```c++
#include <cu0/proc/process.hh>

int main() {
  //! @note not supported on all platforms yet
  //! @note executable can be run by a process
  const auto executable = cu0::Executable{ .binary = "a.out" };
  //! @note variant contains a process if it was created and error code else
  const std::variant<cu0::Process, cu0::Process::CreateError> variant =
      cu0::Process::create(executable);
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: No processes were created" << '\n';
  } else {
    //! @note contains a representation of the running executable
    const auto& created_process = std::get<cu0::Process>(variant);
    std::cout << "Pid of the created process: " << created_process.pid() << 
        '\n';
  }
}
```

#### Get a representation of the current process

`examples/example_cu0_process_current.cc`
```c++
#include <cu0/proc/process.hh>

int main() {
  //! @note not supported on all platforms yet
  //! @note contains a representation of the current process
  const auto this_process = cu0::Process::current();
  std::cout << "Pid of this process: " << this_process.pid() << '\n';
}
```

#### Get the exit status code of a process

`examples/example_cu0_process_exit_code.cc`
```c++
#include <cu0/proc/process.hh>

int main() {
  auto variant = cu0::Process::create(cu0::Executable{
    .binary = "some_executable"
  });
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: the process was not created" << '\n';
  }
  auto& some_process = std::get<cu0::Process>(variant);
  some_process.wait();
  //! @note not supported on all platforms yet
  //! @note exit code can be obtained only after a call to the wait function
  //! @note exit code contains exit status code of the created process
  const auto& exit_code = some_process.exit_code();
  if (!exit_code.has_value()) {
    std::cout << "The exit code was not obtained" << '\n';
  } else {
    std::cout << "Exit code of the created process: " << *exit_code << '\n';
  }
}
```

#### Get stdout (or stderr) of a process

`examples/example_cu0_process_stdout.cc`
```c++
#include <cu0/proc/process.hh>
#include <iostream>

int main() {
  const auto variant = cu0::Process::create(cu0::Executable{
    .binary = "some_executable"
  });
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: the process was not created" << '\n';
  }
  const auto& some_process = std::get<cu0::Process>(variant);
  //! @note not supported on all platforms yet
  //! @note stdout contains standard output of the created process
  //!     at the moment of call
  const auto out_str = some_process.stdout();
  if (out_str.empty()) {
    std::cout << "Error or stdout of the created process is empty" << '\n';
  } else {
    std::cout << "Stdout of the created process: " << out_str << '\n';
  }
}
```

#### Pass data to stdin of a process

`examples/example_cu0_process_stdin.cc`
```c++
#include <cu0/proc/process.hh>
#include <iostream>

int main() {
  const auto variant = cu0::Process::create(cu0::Executable{
    .binary = "some_executable"
  });
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: the process was not created" << '\n';
  }
  const auto& some_process = std::get<cu0::Process>(variant);
  //! @note not supported on all platforms yet
  //! @note stdin passes an input to the stdin of the process
  some_process.stdin("someInput");
}
```

#### Send termination signal to a process

`examples/example_cu0_process_signal.cc`
```c++
#include <cu0/proc/process.hh>
#include <iostream>

int main() {
  const auto variant = cu0::Process::create(cu0::Executable{
    .binary = "some_executable"
  });
  if (!std::holds_alternative<cu0::Process>(variant)) {
    std::cout << "Error: the process was not created" << '\n';
  }
  const auto& some_process = std::get<cu0::Process>(variant);
  //! @note not supported on all platforms yet
  //! @note signals the SIGTERM signal to the process
  some_process.signal(SIGTERM);
}
```

### cu0::Strand

Structure allowing control of a thread of execution

#### Create a strand

`examples/example_cu0_strand_create.cc`
```c++
#include <cu0/proc/strand.hh>
#include <iostream>

int main() {
  const auto variant = cu0::Strand::create([](){ return; });
  if (!std::holds_alternative<cu0::Strand>(variant)) {
    std::cerr << "Error: the strand couldn't be created" << '\n';
  }
  const auto& strand = std::get<cu0::Strand>(variant);
}
```

### Set scheduling parameters of a strand

`examples/example_cu0_strand_scheduling.cc`
```c++
#include <cu0/proc/strand.hh>
#include <iostream>

int main() {
  auto variant = cu0::Strand::create([](){ return; });
  if (!std::holds_alternative<cu0::Strand>(variant)) {
    std::cerr << "Error: the strand couldn't be created" << '\n';
    return 1;
  }
  auto& strand = std::get<cu0::Strand>(variant);
  const auto policy_to_set = cu0::Strand::Policy::PTHREAD_OTHER;
  const auto min_priority =
      sched_get_priority_min(static_cast<int>(policy_to_set));
  const auto set_scheduling_result =
      strand.scheduling<cu0::Strand::Stage::NOT_LAUNCHED>({
        .policy = policy_to_set,
        .priority = min_priority,
      });
  if (!std::holds_alternative<std::monostate>(set_scheduling_result)) {
    std::cout << "Error: the scheduling of the strand couldn't be modified" <<
        '\n';
    return 2;
  }
  const auto run_result = strand.run();
  if (!std::holds_alternative<std::monostate>(run_result)) {
    std::cout << "Error: the strand couldn't be launched" << '\n';
    return 3;
  }
  const auto get_scheduling_result =
      strand.scheduling<cu0::Strand::Stage::LAUNCHED>();
  if (!std::holds_alternative<typename cu0::Strand::Scheduling>(
      get_scheduling_result
  )) {
    std::cout << "Error: the scheduling of the strand couldn't be retrieved" <<
        '\n';
  } else {
    const auto& scheduling =
        std::get<typename cu0::Strand::Scheduling>(get_scheduling_result);
    std::cout << "Info: the strand has scheduling == {" << '\n';
    std::string policy_string;
    switch (static_cast<int>(scheduling.policy)) {
    case SCHED_OTHER:
      policy_string = "SCHED_OTHER";
      break;
    case SCHED_FIFO:
      policy_string = "SCHED_FIFO";
      break;
    case SCHED_RR:
      policy_string = "SCHED_RR";
      break;
    default:
      policy_string = "?";
      break;
    }
    std::cout << "  .policy = " << policy_string << "," <<
        '\n';
    std::cout << "  .priority = " << scheduling.priority << "," << '\n';
    std::cout << "}" << '\n';
  }
  const auto join_result = strand.join();
  if (!std::holds_alternative<std::monostate>(join_result)) {
    std::cout << "Error: the strand couldn't be joined" << '\n';
  }
}
```

### cu0::BlockCoarseTimer

#### Wait for a timer by sleeping

`examples/example_cu0_block_coarse_timer.cc`
```c++
#include <cu0/time/block_coarse_timer.hh>
#include <iostream>

int main() {
  //! create timer set for approximately 2 seconds
  constexpr auto timer = cu0::BlockCoarseTimer<std::int64_t, std::ratio<1, 1>>{
    std::chrono::duration<std::int64_t>{2}
  };
  //! measure start time
  const auto start = std::chrono::high_resolution_clock::now();
  //! launch timer
  timer.launch(); //! will block until the timer is up
  //! measure elapsed time
  const auto elapsed = std::chrono::duration_cast<
      std::chrono::duration<double>
  >(std::chrono::high_resolution_clock::now() - start);
  
  std::cout << "Elapsed: " << elapsed << '\n';
}
```

### cu0::AsyncCoarseTimer

#### Wait for a timer by sleeping

`examples/example_cu0_async_coarse_timer.cc`
```c++
#include <cu0/time/async_coarse_timer.hh>
#include <iostream>

int main() {
  //! create timer set for approximately 2000 milliseconds
  auto timer = cu0::AsyncCoarseTimer<std::int64_t, std::milli>{
    std::chrono::duration<std::int64_t, std::milli>{2000}
  };
  //! measure start time
  const auto start = std::chrono::high_resolution_clock::now();
  //! launch the timer
  timer.launch();
  //! wait for the timer
  timer.wait(); //! will block until the timer is up
  //! measure elapsed time
  const auto elapsed = std::chrono::duration_cast<
      std::chrono::duration<double>
  >(std::chrono::high_resolution_clock::now() - start);

  std::cout << "Elapsed: " << elapsed << '\n';
}
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

Distributed under the `MIT-0` license. See license.txt for more information.
