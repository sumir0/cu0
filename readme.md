# cu0

cu0 is a header-only library providing common utilities for software engineering.

## Testing

```console
cmake -S <path-to-local-repository> -B <path-to-build-directory>
make -C <path-to-build-directory>
make -C <path-to-build-directory> test
```

## Features

### cu0::EnvironmentVariable

#### Get an environment variable value

`examples/example_cu0_environment_variable_cached_value.cc`
```c++
#include <cu0/env.hxx>
#include <iostream>

int main() {
  //! @note environment variable may not be set
  //! read value of the environment variable with the key 'KEY'
  //! @note if the environment variable is not set ->
  //!     empty string is returned
  //! @note if the returned cached value is empty ->
  //!     it does not mean that the environment variable was not set,
  //!     it may be set to the empty value
  //! cachedValue contains value of the environment variable at the time of
  //!     cu0::EnvironmentVariable instance construction
  const auto& cachedValue = cu0::EnvironmentVariable{"KEY"}.cachedValue();
  std::cout << "Value of the environment variable 'KEY': " <<
      (cachedValue.empty() ? "<empty-or-not-set>" : cachedValue) << '\n';
}
```

#### Synchronise an environment variable value

`examples/example_cu0_environment_variable_sync.cc`
```c++
#include <cu0/env.hxx>
#include <iostream>

int main() {
  //! @note environment variable may not be set
  auto environmentVariable = cu0::EnvironmentVariable{"KEY"};
  //! wait for external environment variable change and
  //! sync value of the environment variable with the key 'KEY'
  const auto& syncedValue = cu0::EnvironmentVariable{"KEY"}.sync();
  std::cout << "Synced value of the environment variable 'KEY': " <<
      (syncedValue.empty() ? "<empty-or-not-set>" : syncedValue) << '\n';
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
  const auto executable =
      cu0::util::findBy("someExecutableName", std::filesystem::current_path());
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
  const auto executable = cu0::util::findBy("someExecutableName");
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
#include <cu0/proc.hxx>

int main() {
  //! @note not supported on all platforms yet
  //! @note executable can be run by a process
  const auto executable = cu0::Executable{ .binary = "a.out" };
  //! @note optional contains a process if it was created
  const std::optional<cu0::Process> optional =
      cu0::Process::create(executable);
  if (!optional.has_value()) {
    std::cout << "Error: No processes were created" << '\n';
  } else {
    //! @note createdProcess contains a representation of the running executable
    const auto& createdProcess = *optional;
    std::cout << "Pid of the created process: " << createdProcess.pid() << '\n';
  }
}
```

#### Get a representation of the current process

`examples/example_cu0_process_current.cc`
```c++
#include <cu0/proc.hxx>

int main() {
  //! @note not supported on all platforms yet
  //! @note thisProcess contains a representation of the current process
  const auto thisProcess = cu0::Process::current();
  std::cout << "Pid of this process: " << thisProcess.pid() << '\n';
}
```

#### Get the exit status code of a process

`examples/example_cu0_process_exit_code.cc`
```c++
#include <cu0/proc.hxx>

int main() {
  const auto someProcess = cu0::Process::create(cu0::Executable{
    .binary = "someExecutable"
  });
  if (!someProcess.has_value()) {
    std::cout << "Error: the process was not created" << '\n';
  }
  //! @note not supported on all platforms yet
  //! @note exit code contains exit status code of the created process
  const auto exitCode = someProcess->exitCode();
  if (!exitCode.has_value()) {
    std::cout << "Error: the exit code was not obtained" << '\n';
  } else {
    std::cout << "Exit code of the created process: " << *exitCode << '\n';
  }
}
```

#### Get a process identifier

`examples/example_cu0_process_pid.cc`
```c++
#include <cu0/proc.hxx>
#include <iostream>

int main() {
  const auto thisProcess = cu0::Process::current();
  //! @note pid contains value of the process identifier of the current process
  const auto& pid = thisProcess.pid();
  std::cout << "This process identifier: " << pid << '\n';
}
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

Distributed under the `MIT-0` license. See license.txt for more information.
