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
