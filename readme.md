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

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

Distributed under the `MIT-0` license. See license.txt for more information.
