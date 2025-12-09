# cu0

## Abstract

cu0 is a header-only common utilities library for software engineering.

## Contents

	About
	Usage
	Libraries
		Environment
			cu0::Environment
			cu0::EnvironmentVariableData
			cu0::EnvironmentVariable
		Platform
			not_an_x
		Process
			cu0::Executable
			cu0::Process
			cu0::Strand
		Time
			cu0::AsyncCoarseTimer
			cu0::BlockCoarseTimer

## About

> **_SEE:_** `readme.md:Testing` for information on building and testing.

> **_SEE:_** `readme.md:Features` for a brief tour of features.

## Usage

cu0 can be used as a library or as a set of libraries.

To use it as a library, include `cu0/cu0.hxx` header.
```c++
#include <cu0/cu0.hxx>
```
That will include all other libraries.

To use it as a set of libraries, include only required to use libraries.
```c++
#include <cu0/some_cu0_library.hxx>
```
That will include only specified libraries and their dependencies if any.

## Libraries

| Library  |       Header       |                      Description |
| -------: | :----------------: | :------------------------------- |
|      env |   `cu0/env.hxx`    | Process environment control      |
| platform | `cu0/platform.hxx` | Platform-dependent helpers       |
|     proc |   `cu0/proc.hxx`   | Process management               |
|     time |   `cu0/time.hxx`   | Time-related functionality       |
|      cu0 |   `cu0/cu0.hxx`    | Contains all the other libraries |

---

### Environment

---

#### `struct cu0::Environment`

---

```c++
struct cu0::Environment;
```

The Environment struct provides a way to access the environment

> **_NOTE:_** no instances of this struct can be created

> **_NOTE:_** for a way to access the environment of execution 

>> **_SEE:_** cu0::Environment::as()

---

```c++
public:
constexpr cu0::Environment() = delete;
```

no instances of this struct can be created.

---

```c++
#if __has_include(<unistd.h>)
public:
template <class Return>
[[nodiscard]]
constexpr static Return cu0::Environment::as() = delete;
#endif
```

accesses the environment of execution and returns it as the specified type

> **_NOTE:_** this function is marked deleted to allow only usage of specializations

_Template parameters_

Return is the type that will be returned

_Returns_

environment as Return type

---

```c++
#if __has_include(<unistd.h>)
public:
template <>
[[nodiscard]]
std::map<std::string, std::string> cu0::Environment::as();
#endif
```

> **_NOTE:_** specialization of cu0::Environment::as()

copies environment variables into `std::map<std::string, std::string>`

> **_NOTE:_** the behaviour is undefined if environment changes during function execution

_Returns_

a map of the current environment variables in the form `<key, value>`

---

```c++
#if __has_include(<unistd.h>)
public:
template <>
[[nodiscard]]
std::vector<EnvironmentVariable> cu0::Environment::as();
#endif
```

> **_NOTE:_** specialization of cu0::Environment::as()

copies environment variables into `std::vector<EnvironmentVariable>`

> **_NOTE:_** the behaviour is undefined if environment changes during function execution

_Returns_

vector of the current environment variables

---

```c++
#if __has_include(<unistd.h>)
protected:
template <class Return>
[[nodiscard]]
constexpr static Return cu0::Environment::as(
    const std::function<void(
        Return&, 
        std::string&&, 
        std::string&&
    )>& insert
);
#endif
```

accesses the environment of execution and returns it as the specified type

_Template parameters_

Return is the type that will be returned

_Parameters_

insert is the function to insert key-value data

`Return&` param is the first parameter of the function into which key-value data needs to be inserted

`std::string&&` is the second parameter of the function representing key of the environment variable

`std::string&&` is the third parameter of the function representing value of the environment variable

_Returns_

environment as Return type

---

#### `struct cu0::EnvironmentVariableData`

---

```c++
struct cu0::EnvironmentVariableData;
```

The EnvironmentVariableData struct provides a way to represent environment variables in a memory

---

```c++
public:
std::string cu0::EnvironmentVariableData::key{};
```

key of the associated environment variable

---

```c++
public:
std::optional<std::string> cu0::EnvironmentVariableData::value{};
```

value of the associated environment variable

---

#### `struct cu0::EnvironmentVariable`

---

```c++
struct cu0::EnvironmentVariable;
```

The EnvironmentVariable struct provides a way to access environment variable values

---

```c++
#ifndef not_an_x
public:
enum class cu0::EnvironmentVariable::SetError;
#endif
```

enum of possible errors during value setting

---

```c++
#ifndef not_an_x
cu0::EnvironmentVariable::SetError::INVALID
#endif
```

data_.key is a string of length 0, or contains an '=' character

---

```c++
#ifndef not_an_x
cu0::EnvironmentVariable::SetError::MEMORY
#endif
```

insufficient memory to add a new variable to the environment

---

```c++
public:
virtual constexpr
cu0::EnvironmentVariable::~EnvironmentVariable() = default;
```

destructor that may be used in inherited structs

---

```c++
public:
[[nodiscard]]
static constexpr cu0::EnvironmentVariable
cu0::EnvironmentVariable::unsynced(std::string key);
```

creates an instance without syncing with the environment

_Parameters_

key is the environment variable key

_Returns_

unsynced environment variable

---

```c++
public:
[[nodiscard]]
static constexpr cu0::EnvironmentVariable
cu0::EnvironmentVariable::unsynced(
    cu0::EnvironmentVariableData data
);
```

creates an instance without syncing with the environment

_Parameters_

data is the environment variable data, i.e. key and value

_Returns_

unsynced environment variable with the value cached from the data

---

```c++
public:
[[nodiscard]]
static cu0::EnvironmentVariable
cu0::EnvironmentVariable::synced(std::string key);
```

creates an instance syncing with the environment

_Parameters_

key is the environment variable key

_Returns_

synced environment variable

---

```c++
public:
[[nodiscard]]
constexpr const cu0::EnvironmentVariableData&
cu0::EnvironmentVariable::data() const;
```

accesses the data of this instance 

> **_SEE:_** cu0::EnvironmentVariable::data_

_Returns_

data as a const reference

---

```c++
public:
[[nodiscard]]
constexpr const std::string& cu0::EnvironmentVariable::key() const;
```

accesses the key of an associated environment variable

_Returns_

key as a const reference

---

```c++
public:
[[nodiscard]]
constexpr const std::optional<std::string>&
cu0::EnvironmentVariable::cached() const;
```

accesses the cached value of the associated environment variable

> **_NOTE:_** the cached value may not represent the associated environment variable if it was modified after the construction of this instance and before a call to this function

_Returns_

cached value as a const reference

---

```c++
public:
const std::optional<std::string>& cu0::EnvironmentVariable::sync();
```

syncs cached value to the actual value of the associated environment variable

_Returns_

new cached value as a const reference

---

```c++
#ifndef not_an_x
public:
std::variant<
    std::monostate, 
    cu0::EnvironmentVariable::SetError
> cu0::EnvironmentVariable::set(std::string value);
#endif
```

sets the value of the associated environment variable

_Parameters_

value is the value to be set

_Returns_

if no error was reported => std::monostate

else => error code

> **_SEE:_** cu0::EnvironmentVariable::SetError

---

```c++
#ifndef not_an_x
public:
std::variant<
    std::monostate, 
    cu0::EnvironmentVariable::SetError
> cu0::EnvironmentVariable::unset();
#endif
```

unsets the value of the associated environment variable

_Returns_

if no error was reported => std::monostate

else => error code

> **_SEE:_** cu0::EnvironmentVariable::SetError

---

```c++
#ifndef not_an_x
protected:
template <class ConvertTo>
static constexpr ConvertTo cu0::EnvironmentVariable::convert(
    const int& errorNumber
);
#endif
```

converts an error number (error code) to the ConvertTo type

_Template parameters_

ConvertTo is a struct conversion to which is needed

> **_NOTE:_** ConvertTo may be one of `{ SetError, }`

_Parameters_

errorNumber is the error number (error code)

_Returns_

converted value

---

```c++
protected:
constexpr cu0::EnvironmentVariable::EnvironmentVariable() = default;
```

constructs an instance without syncing with the environment

> **_NOTE:_** does not access an actual environment variable value

---

```c++
protected:
EnvironmentVariableData cu0::EnvironmentVariable::data_{};
```

key-value data of the associated environment variable

---

### Platform

---

#### `not_an_x`

---

not_an_x is defined if this platform was detected as not an x platform

> **_EXAMPLE:_** linux

> **_EXAMPLE:_** unix

```c++
#ifndef __unix__
#ifdef not_an_x
#warning not_an_x has been aready defined somewhere else => not redefining it
#else
#define not_an_x
#endif
#endif
```

---

### Proc

---

#### `struct cu0::Executable`

---

```c++
struct cu0::Executable;
```

The Executable struct represents an executable stored in a filesystem with arguments (if any) inside an environment

---

```c++
std::filesystem::path cu0::Executable::binary{};
```

path to a binary

---

```c++
std::vector<std::string> cu0::Executable::arguments{};
```

argument vector

---

```c++
std::map<std::string, std::string> cu0::Executable::environment{};
```

environment in the form <key, value>

---

##### `namespace cu0::util`

---

```c++
[[nodiscard]]
cu0::Executable cu0::findBy(const std::string& name);
```

finds an executable by a name

> **_NOTE:_** searches in order:

>> the current directory

>> the directories specified by the PATH environment variable

_Parameters_

name is the name of an executable

_Returns_

executable with empty arguments and an empty environment

> **_NOTE:_** if multiple executables are present with the specified name => the first found executable is returned

---

```c++
[[nodiscard]]
cu0::Executable cu0::findBy(
    const std::string& name,
    const std::filesystem::path& directory
);
```

finds an executable by a name

_Parameters_

name is the name of an executable

directory is the directory where to look for an executable

_Returns_

executable with empty arguments and an empty environment

---

```c++
[[nodiscard]]
std::tuple<std::unique_ptr<std::unique_ptr<char[]>[]>, std::size_t> cu0::argvOf(
    const cu0::Executable& executable
);
```

converts arguments of an executable to `ptr<ptr<char[]>[]>`

_Parameters_

executable is the executable, arguments of which will be converted

directory is the directory where to look for an executable

_Returns_

tuple containing a ptr to an array and the size of that array

---

```c++
[[nodiscard]]
std::tuple<std::unique_ptr<std::unique_ptr<char[]>[]>, std::size_t> cu0::argvOf(
    const cu0::Executable& executable
);
```

converts environment of an executable to `ptr<ptr<char[]>[]>`

> **_NOTE:_** in format of `"key=value"`

_Parameters_

executable is the executable, environment of which will be converted

_Returns_

tuple containing a ptr to an array and the size of that array

---

#### `struct cu0::Process`

---

```c++
#if __has_include(<unistd.h>)
public:
enum struct cu0::Process::CreateError;
#endif
```

enum of possible errors for `cu0::Process::create()` and `cu0::Process::createPipeless()` functions

---

```c++
cu0::Process::CreateError::AGAIN = EAGAIN,
```
> **_SEE:_** `EAGAIN`

---

```c++
cu0::Process::CreateError::NOMEM = ENOMEM,
```
> **_SEE:_** `ENOMEM`

---

```c++
cu0::Process::CreateError::INVAL = EINVAL,
```
> **_SEE:_** `EINVAL`

---

```c++
cu0::Process::CreateError::MFILE = EMFILE,
```
> **_SEE:_** `EMFILE`

---

```c++
cu0::Process::CreateError::NFILE = ENFILE,
```
> **_SEE:_** `ENFILE`

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
public:
enum struct cu0::Process::WaitError;
#endif
```

enum of possible errors for `cu0::Process::waitCautious()` function

```c++
cu0::Process::WaitError::CHILD = ECHILD,
```
> **_SEE:_** `ECHILD`

---

```c++
cu0::Process::WaitError::INVAL = EINVAL,
```
> **_SEE:_** `EINVAL`

---

```c++
cu0::Process::WaitError::INTR = EINTR,
```
> **_SEE:_** `EINTR`

---

```c++
#if __has_include(<unistd.h>)
public:
enum struct cu0::Process::WriteError;
#endif
```

enum of possible errors for `cu0::Process::stdinCautious()` function

```c++
cu0::Process::WriteError::AGAIN = EAGAIN,
```
> **_SEE:_** `EAGAIN`

---

```c++
#if EWOULDBLOCK != EAGAIN
cu0::Process::WriteError::WOULDBLOCK = EWOULDBLOCK,
#endif
```
> **_SEE:_** `EWOULDBLOCK`

---

```c++
cu0::Process::WriteError::BADF = EBADF,
```
> **_SEE:_** `EBADF`

---

```c++
cu0::Process::WriteError::DESTADDRREQ = EDESTADDRREQ,
```
> **_SEE:_** `EDESTADDRREQ`

---

```c++
cu0::Process::WriteError::DQUOT = EDQUOT,
```
> **_SEE:_** `EDQUOT`

---

```c++
cu0::Process::WriteError::FAULT = EFAULT,
```
> **_SEE:_** `EFAULT`

---

```c++
cu0::Process::WriteError::FBIG = EFBIG,
```
> **_SEE:_** `EFBIG`

---

```c++
cu0::Process::WriteError::INTR = EINTR,
```
> **_SEE:_** `EINTR`

---

```c++
cu0::Process::WriteError::INVAL = EINVAL,
```
> **_SEE:_** `EINVAL`

---

```c++
cu0::Process::WriteError::IO = EIO,
```
> **_SEE:_** `EIO`

---

```c++
cu0::Process::WriteError::NOSPC = ENOSPC,
```
> **_SEE:_** `ENOSPC`

---

```c++
cu0::Process::WriteError::PERM = EPERM,
```
> **_SEE:_** `EPERM`

---

```c++
cu0::Process::WriteError::PIPE = EPIPE,
```
> **_SEE:_** `EPIPE`

---

```c++
#if __has_include(<unistd.h>)
public:
enum struct cu0::Process::ReadError;
#endif
```

enum of possible errors for `cu0::Process::stdoutCautious()` and `cu0::Process::stderrCautious()` functions

---

```c++
cu0::Process::ReadError::AGAIN = EAGAIN,
```
> **_SEE:_** `EAGAIN`

---

```c++
cu0::Process::ReadError::BADF = EBADF,
```
> **_SEE:_** `EBADF`

---

```c++
cu0::Process::ReadError::BADMSG = EBADMSG,
```
> **_SEE:_** `EBADMSG`

---

```c++
cu0::Process::ReadError::INTR = EINTR,
```
> **_SEE:_** `EINTR`

---

```c++
cu0::Process::ReadError::INVAL = EINVAL,
```
> **_SEE:_** `EINVAL`

---

```c++
cu0::Process::ReadError::IO = EIO,
```
> **_SEE:_** `EIO`

---

```c++
cu0::Process::ReadError::ISDIR = EISDIR,
```
> **_SEE:_** `EISDIR`

---

```c++
cu0::Process::ReadError::OVERFLOW = EOVERFLOW,
```
> **_SEE:_** `EOVERFLOW`

---

```c++
#if EWOULDBLOCK != EAGAIN
cu0::Process::ReadError::WOULDBLOCK = EWOULDBLOCK,
#endif
```
> **_SEE:_** `EWOULDBLOCK`

---

```c++
cu0::Process::ReadError::CONNRESET = ECONNRESET,
```
> **_SEE:_** `ECONNRESET`

---

```c++
cu0::Process::ReadError::NOTCONN = ENOTCONN,
```
> **_SEE:_** `ENOTCONN`

---

```c++
cu0::Process::ReadError::TIMEDOUT = ETIMEDOUT,
```
> **_SEE:_** `ETIMEDOUT`

---

```c++
cu0::Process::ReadError::NOBUFS = ENOBUFS,
```
> **_SEE:_** `ENOBUFS`

---

```c++
cu0::Process::ReadError::NOMEM = ENOMEM,
```
> **_SEE:_** `ENOMEM`

---

```c++
cu0::Process::ReadError::NXIO = ENXIO,
```
> **_SEE:_** `ENXIO`

---

```c++
#if __has_include(<signal.h>)
public:
enum struct cu0::Process::SignalError;
#endif
```

enum of possible errors for `cu0::Process::signalCautious()` function

---

```c++
cu0::Process::SignalError::INVAL = EINVAL,
```
> **_SEE:_** `EINVAL`

---

```c++
cu0::Process::SignalError::PERM = EPERM,
```
> **_SEE:_** `EPERM`

---

```c++
cu0::Process::SignalError::SRCH = ESRCH,
```
> **_SEE:_** `ESRCH`

---

```c++
#if __has_include(<unistd.h>)
public:
[[nodiscard]]
static cu0::Process cu0::Process::current();
#endif
```

constructs an instance using the current process in which this function is called

> **_NOTE:_** for the returned current process cu0::Process::stdin(), cu0::Process::stdout(), cu0::Process::stderr() member functions are not supported yet

>> **_SEE:_** implementation details of cu0::Process::current()

_Returns_

current process

---

```c++
#if __has_include(<unistd.h>)
public:
[[nodiscard]]
static std::variant<cu0::Process, cu0::Process::CreateError>
cu0::Process::create(const cu0::Executable& executable);
#endif
```

creates a process using the specified executable

_Parameters_

executable is the excutable to be run by the process

_Returns_

if no error was reported => created process

else => error code

---

```c++
#if __has_include(<unistd.h>)
public:
[[nodiscard]]
static std::variant<cu0::Process, cu0::Process::CreateError>
cu0::Process::createPipeless(const cu0::Executable& executable);
#endif
```

creates a process using the specified executable without pipes

_Parameters_

executable is the excutable to be run by the process

_Returns_

if no error was reported => created process

else => error code

---

```c++
public:
virtual cu0::Process::~Process();
```

destructs an instance

---

```c++
public:
constexpr cu0::Process(const cu0::Process& other) = delete;
```

---

```c++
public:
constexpr cu0::Process& cu0::Process::operator =(
    const cu0::Process& other
) = delete;
```

---

```c++
public:
constexpr cu0::Process(cu0::Process&& other);
```

moves process resources to this process

_Parameters_

other is the process for which resources need to be moved

---

```c++
public:
constexpr cu0::Process& cu0::Process::operator =(cu0::Process&& other);
```

moves process resources to this process

_Parameters_

other is the process for which resources need to be moved

_Returns_

this process as a mutable reference

---

```c++
public:
[[nodiscard]]
constexpr const unsigned& cu0::Process::pid() const;
```

accesses process identifier value

_Returns_

process identifier as a const reference

---

```c++
public:
[[nodiscard]]
constexpr std::optional<int> cu0::Process::stdinPipe() const;
```

accesses stdin pipe file descriptor

_Returns_

if stdin pipe file descriptor is valid => its value

else => empty optional

---

```c++
public:
[[nodiscard]]
constexpr std::optional<int> cu0::Process::stdoutPipe() const;
```

accesses stdout pipe file descriptor

_Returns_

if stdout pipe file descriptor is valid => its value

else => empty optional

---

```c++
public:
[[nodiscard]]
constexpr std::optional<int> cu0::Process::stderrPipe() const;
```

accesses stderr pipe file descriptor

_Returns_

if stderr pipe file descriptor is valid => its value

else => empty optional

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
public:
void cu0::Process::wait();
#endif
```

waits for the process to exit or to be terminated or to be stopped

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
public:
[[nodiscard]]
std::variant<std::monostate, cu0::Process::WaitError>
cu0::Process::waitCautious();
#endif
```

waitCautious waits for the process to exit or to be terminated or to be stopped

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
public:
[[nodiscard]]
constexpr const std::optional<int>& cu0::Process::exitCode() const;
#endif
```

accesses exit status code

> **_NOTE:_** exit status code will be empty if the process hasn't exited normally

>> **_SEE:_** cu0::Process::wait()

_Returns_

exit status code as a const reference

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
public:
[[nodiscard]]
constexpr const std::optional<int>& cu0::Process::terminationCode() const;
#endif
```

accesses termination signal code

> **_NOTE:_** termination signal code will be empty if the process hasn't been terminated

>> **_SEE:_** cu0::Process::wait()

_Returns_

termination signal code as a const reference

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
public:
[[nodiscard]]
constexpr const std::optional<int>& cu0::Process::stopCode() const;
#endif
```

accesses stop signal code

> **_NOTE:_** stop signal code will be empty if the process hasn't been stopped

>> **_SEE:_** cu0::Process::wait()

_Returns_

stop signal code as a const reference

---

```c++
#if __has_include(<unistd.h>)
public:
void cu0::Process::stdin(const std::string& input) const;
#endif
```

passes the specified input to the stdin

_Parameters_

input is the input value

---

```c++
#if __has_include(<unistd.h>)
public:
[[nodiscard]]
std::tuple<
    std::variant<std::monostate, cu0::Process::WriteError>, 
    std::size_t
> cu0::Process::stdinCautious(const std::string& input) const;
#endif
```

passes the specified input to the stdin

_Parameters_

input is the input value

_Returns_

result of cu0::Process::writeInto()

> **_SEE:_** cu0::Process::writeInto()

---

```c++
#if __has_include(<unistd.h>)
public:
[[nodiscard]]
std::string cu0::stdout() const;
#endif
```

returns the value of the stdout

_Returns_

string containing stdout value

---

```c++
#if __has_include(<unistd.h>)
public:
[[nodiscard]]
std::tuple<
    std::variant<std::monostate, cu0::Process::ReadError>, 
    std::string
> cu0::Process::stdoutCautious() const;
#endif
```

returns the value of the stdout

_Returns_

result of cu0::Process::readFrom()

> **_SEE:_** cu0::Process::readFrom()

---

```c++
#if __has_include(<unistd.h>)
public:
[[nodiscard]]
std::string cu0::stderr() const;
#endif
```

returns the value of the stderr

_Returns_

string containing stderr value

---

```c++
#if __has_include(<unistd.h>)
public:
[[nodiscard]]
std::tuple<
    std::variant<std::monostate, cu0::Process::ReadError>, 
    std::string
> cu0::Process::stderrCautious() const;
#endif
```

returns the value of the stderr

_Returns_

result of cu0::Process::readFrom()

> **_SEE:_** cu0::Process::readFrom()

---

```c++
#if __has_include(<signal.h>)
public:
void cu0::Process::signal(const int& code) const;
#endif
```

sends the specified code as a signal to this process

_Parameters_

code is the signal to be sent

---

```c++
#if __has_include(<signal.h>)
public:
[[nodiscard]]
std::variant<std::monostate, cu0::Process::SignalError>
cu0::Process::signalCautious(const int& code) const;
#endif
```

sends the specified code as a signal to this process

_Parameters_

code is the signal to be sent

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<unistd.h>)
protected:
template <std::size_t BUFFER_SIZE, class Return>
static Return cu0::Process::writeInto(
    const int& pipe,
    const std::string& input
);
#endif
```

writes the specified input into the specified pipe

_Template parameters_

BUFFER_SIZE is the buffer size for writing into the pipe

Return is the type to be returned by this function

_Parameters_

pipe is the pipe to write into

input is the data to write

_Returns_

```c++
if Return == std::tuple<
    std::variant<std::monostate, cu0::Process::WriteError>,
    std::size_t
> =>
    tuple containing
        variant of
            if no error was reported => std::monostate
            else => error code
        number of bytes written
            if no error was reported =>
                equal to input size
            else =>
                equal to the number of bytes that had already been
                written before an error was reported
if Return == void => nothing
```

---

```c++
#if __has_include(<unistd.h>)
protected:
template <std::size_t BUFFER_SIZE, class Return>
[[nodiscard]]
static Return cu0::Process::readFrom(const int& pipe);
#endif
```

reads from the specified pipe

_Template parameters_

BUFFER_SIZE is the buffer size for reading from the pipe

Return is the type to be returned by this function

_Parameters_

pipe is the pipe to read from

_Returns_

```c++
if Return == std::tuple<
    std::variant<std::monostate, cu0::Process::ReadError>,
    std::string
> =>
    tuple containing
        variant of
            if no error was reported => std::monostate
            else => error code
        data that have been read
            if no error was reported => fully received
            else =>
                partially and contains data that had already been
                read before an error was reported
if Return == std::string => data that have been read as std::string
```

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
protected:
template <class Return>
Return cu0::Process::waitExitLoop();
#endif
```

loop to wait for process exit

_Template parameters_

Return is the type to be returned by this function

_Parameters_

pipe is the pipe to read from

_Returns_

```c++
if Return == std::variant<std::monostate, cu0::Process::WaitError> =>
    if no error was reported => std::monostate
    else =>
        error code of the first encountered error
if Return == void => no errors are returned and handled
```

---

```c++
protected:
constexpr cu0::Process::Process() = default;
```

constructs an instance with default values

---

```c++
protected:
constexpr void cu0::Process::swap(cu0::Process&& other);
```

swaps two processes

_Parameters_

other is the process to swap this process with

---

```c++
protected:
unsigned cu0::Process::pid_ = 0;
```

process identifier

---

```c++
protected:
int cu0::Process::stdinPipe_ = -1;
```

stdin file descriptor

---

```c++
protected:
int cu0::Process::stdoutPipe_ = -1;
```

stdout file descriptor

---

```c++
protected:
int cu0::Process::stderrPipe_ = -1;
```

stderr file descriptor

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
protected:
std::optional<int> cu0::Process::exitCode_ = {};
#endif
```

if waited => actual exit status code value if present

> **_SEE:_** cu0::Process::wait()

else => empty exit status code value

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
protected:
std::optional<int> cu0::Process::terminationCode_ = {};
#endif
```

if waited => actual termination signal code value if present

> **_SEE:_** cu0::Process::wait()

else => empty termination signal code value

---

```c++
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
protected:
std::optional<int> cu0::Process::stopCode_ = {};
#endif
```

if waited => actual stop signal code value if present

> **_SEE:_** cu0::Process::wait()

else => empty stop signal code value

---

#### `struct cu0::Strand`

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::Policy;
#endif
```

available policies

---

```c++
cu0::Strand::Policy::PTHREAD_OTHER = SCHED_OTHER,
```
> **_SEE:_** `SCHED_OTHER`

---

```c++
cu0::Strand::Policy::PTHREAD_FIFO = SCHED_FIFO,
```
> **_SEE:_** `SCHED_FIFO`

---

```c++
cu0::Strand::Policy::PTHREAD_RR = SCHED_RR,
```
> **_SEE:_** `SCHED_RR`

---

```c++
#if __has_include(<pthread.h>)
public:
using cu0::Strand::priority_type = std::remove_reference_t<
    decltype(std::declval<sched_param>().sched_priority)
>;
#endif
```

underlying priority type

---

```c++
#if __has_include(<pthread.h>)
public:
struct cu0::Strand::Scheduling;
#endif
```

scheduling data representation

```c++
public:
cu0::Strand::Policy 
cu0::Strand::Scheduling::policy = cu0::Strand::Policy::PTHREAD_OTHER;
```

policy

```c++
public:
cu0::Strand::priority_type cu0::Strand::Scheduling::priority{};
```
> **_NOTE:_** policy-specific
when it is modified use

>> `sched_get_priority_min(static_cast<int>(Policy))`

>> and

>> `sched_get_priority_max(static_cast<int>(Policy))`

>> to get minimal and maximal values for the specified policy

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::Stage;
#endif
```

stage of a strand

---

```c++
cu0::Strand::Stage::NOT_LAUNCHED = SCHED_OTHER,
```

before `cu0::Strand::run()`

---

```c++
cu0::Strand::Stage::LAUNCHED = SCHED_FIFO,
```

after `cu0::Strand::run()` but before `cu0::Strand::join()`

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::ResourceError;
#endif
```

errors related to resource management of a strand

---

```c++
cu0::Strand::ResourceError::PTHREAD_NOMEM = ENOMEM,
```

no memory

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::GetPriorityError;
#endif
```

errors related to retrieval of a priority value

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::SetPriorityError;
#endif
```

errors related to modification of a priority value

---

```c++
cu0::Strand::SetPriorityError::PTHREAD_NOTSUP = ENOTSUP,
```

unsupported priority or policy was specified

---

```c++
cu0::Strand::SetPriorityError::PTHREAD_INVAL = EINVAL,
```

invalid priority or policy was specified

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::GetPolicyError;
#endif
```

errors related to retrieval of a policy value

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::SetPolicyError;
#endif
```

errors related to modification of a policy value

---

```c++
cu0::Strand::SetPolicyError::PTHREAD_NOTSUP = ENOTSUP,
```

unsupported policy was specified

---

```c++
cu0::Strand::SetPolicyError::PTHREAD_INVAL = EINVAL,
```

invalid policy was specified

---

```c++
cu0::Strand::SetPolicyError::PTHREAD_PERM = EPERM,
```

current privileges are not enough

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::InitError;
#endif
```

errors related to initializtion of a strand and its parameters

---

```c++
cu0::Strand::InitError::PTHREAD_NOTSUP = ENOTSUP,
```

operation unsupported on the platform was requested

changing scheduling and/or priority will not make effect

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::RunError;
#endif
```

errors related to launching of a strand

---

```c++
cu0::Strand::RunError::PTHREAD_AGAIN = EAGAIN,
```

insufficient resources or limit

---

```c++
cu0::Strand::RunError::PTHREAD_PERM = EPERM,
```

no permission to apply strand parameters

---

```c++
#if __has_include(<pthread.h>)
public:
enum struct cu0::Strand::JoinError;
#endif
```

errors related to joining to a strand

---

```c++
cu0::Strand::JoinError::PTHREAD_DEADLK = EDEADLK,
```

deadlock was detected

---

```c++
public:
[[nodiscard]] static
#if __has_include(<pthread.h>)
std::variant<
    cu0::Strand, 
    cu0::Strand::ResourceError, 
    cu0::Strand::InitError
>
#else
std::variant<cu0::Strand>
#endif
cu0::Strand::create(std::function<void()> task);
```

creates a strand instance

_Parameters_

task is the task to be executed after launch

> **_SEE:_** cu0::Strand::run()

_Returns_

if no error was reported => strand instance

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <cu0::Strand::Stage stage>
[[nodiscard]]
constexpr std::variant<
    cu0::Strand::priority_type,
    cu0::Strand::GetPriorityError
> cu0::Strand::priority() const = delete;
#endif
```

gets a priority of this strand

> **_NOTE:_** deleted | actual implementations are provided through specializations

_Returns_

if no error was reported => priority value

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <>
[[nodiscard]]
constexpr std::variant<
    cu0::Strand::priority_type,
    cu0::Strand::GetPriorityError
> cu0::Strand::priority<cu0::Strand::Stage::NOT_LAUNCHED>() const;
#endif
```

gets a priority with which this strand will be launched

_Returns_

if no error was reported => priority value

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <>
[[nodiscard]]
constexpr std::variant<
    cu0::Strand::priority_type,
    cu0::Strand::GetPriorityError
> cu0::Strand::priority<cu0::Strand::Stage::LAUNCHED>() const;
#endif
```

gets a priority with which this strand is running

_Returns_

if no error was reported => priority value

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <cu0::Strand::Stage stage>
[[nodiscard]]
constexpr std::variant<
    std::monostate,
    cu0::Strand::SetPriorityError
> cu0::Strand::priority(
    const cu0::Strand::priority_type& priority
) const = delete;
#endif
```

sets a priority of this strand

> **_NOTE:_** deleted | actual implementations are provided through specializations

_Parameters_

priority is the priority to be set

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <>
[[nodiscard]]
constexpr std::variant<
    std::monostate,
    cu0::Strand::SetPriorityError
> cu0::Strand::priority<cu0::Strand::Stage::NOT_LAUNCHED>(
    const cu0::Strand::priority_type& priority
) const;
#endif
```

sets a priority with which this strand will be launched

_Parameters_

priority is the priority to be set

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <>
[[nodiscard]]
constexpr std::variant<
    std::monostate,
    cu0::Strand::SetPriorityError
> cu0::Strand::priority<cu0::Strand::Stage::LAUNCHED>(
    const cu0::Strand::priority_type& priority
) const;
#endif
```

sets a priority with which this strand will continue to run

_Parameters_

priority is the priority to be set

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <cu0::Strand::Stage stage>
[[nodiscard]]
constexpr std::variant<
    cu0::Strand::Scheduling,
    cu0::Strand::GetPolicyError,
    cu0::Strand::GetPriorityError
> cu0::Strand::scheduling() const = delete;
#endif
```

gets scheduling parameters of this strand

> **_NOTE:_** deleted | actual implementations are provided through specializations

_Returns_

if no error was reported => scheduling parameters

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <>
[[nodiscard]]
constexpr std::variant<
    cu0::Strand::Scheduling,
    cu0::Strand::GetPolicyError,
    cu0::Strand::GetPriorityError
> cu0::Strand::scheduling<cu0::Strand::Stage::NOT_LAUNCHED>() const;
#endif
```

gets scheduling parameters with which this strand will be launched

_Returns_

if no error was reported => scheduling parameters

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <>
[[nodiscard]]
constexpr std::variant<
    cu0::Strand::Scheduling,
    cu0::Strand::GetPolicyError,
    cu0::Strand::GetPriorityError
> cu0::Strand::scheduling<cu0::Strand::Stage::LAUNCHED>() const;
#endif
```

gets scheduling parameters with which this strand is running

> **_NOTE:_** cu0::Strand::GetPriorityError is not used by this implementation

_Returns_

if no error was reported => scheduling parameters

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <cu0::Strand::Stage stage>
[[nodiscard]]
constexpr std::variant<
    std::monostate,
    cu0::Strand::SetPolicyError,
    cu0::Strand::SetPriorityError
> cu0::Strand::scheduling(
    const Scheduling& scheduling
) const = delete;
#endif
```

sets scheduling parameters of this strand

> **_NOTE:_** deleted | actual implementations are provided through specializations

_Parameters_

scheduling is the scheduling parameters to be set

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <>
[[nodiscard]]
constexpr std::variant<
    std::monostate,
    cu0::Strand::SetPolicyError,
    cu0::Strand::SetPriorityError
> cu0::Strand::scheduling<cu0::Strand::Stage::NOT_LAUNCHED>(
    const Scheduling& scheduling
) const;
#endif
```

sets scheduling parameters with which this strand will be launched

_Parameters_

scheduling is the scheduling parameters to be set

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
template <>
[[nodiscard]]
constexpr std::variant<
    std::monostate,
    cu0::Strand::SetPolicyError,
    cu0::Strand::SetPriorityError
> cu0::Strand::scheduling<cu0::Strand::Stage::LAUNCHED>(
    const Scheduling& scheduling
) const;
#endif
```

sets scheduling parameters with which this strand will continue to run

> **_NOTE:_** cu0::Strand::SetPriorityError is not used by this implementation

_Parameters_

scheduling is the scheduling parameters to be set

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
std::variant<
    std::monostate,
    cu0::Strand::RunError,
    cu0::Strand::ResourceError
>
#else
std::variant<std::monostate>
#endif
cu0::Strand::run();
```

runs a task specified by `this->task_` in a new thread, i.e. launches this strand

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
#if __has_include(<pthread.h>)
public:
std::variant<
    std::monostate,
    cu0::Strand::JoinError
>
#else
std::variant<std::monostate>
#endif
cu0::Strand::join();
```

joins to this strand, i.e. waits for the strand to exit

_Returns_

if no error was reported => std::monostate

else => error code

---

```c++
protected:
#if __has_include(<pthread.h>)
constexpr
#else
inline
#endif
cu0::Strand::Strand() = default;
```

default ctor

---

```c++
#if __has_include(<pthread.h>)
protected:
static void* task(void* args);
#endif
```

calls `reinterpret_cast<Strand const *>(args)->task_` task

> **_NOTE:_** used as a helper to launch a strand

_Parameters_

args is the pointer to the instance of this type

_Returns_

nullptr

---

```c++
protected:
std::function<void()> cu0::Strand::task_{};
```

task to be executed after launch

---

```c++
#if __has_include(<pthread.h>)
protected:
pthread_attr_t cu0::Strand::attr_{};
#endif
```

internal representation of parameters with which a strand is launched

> **_SEE:_** cu0::Strand::Scheduling

---

```c++
protected:
#if __has_include(<pthread.h>)
pthread_t
#else
std::thread
#endif
cu0::Strand::thread_{};
```

thread implementation

---

### Time

---

#### `cu0::AsyncCoarseTimer`

---

```c++
template <class Rep, class Period>
struct cu0::AsyncCoarseTimer;
```

struct representing asynchronous coarse timer which can be launched

_Template parameters_

Rep is the type representing number of ticks

> **_EXAMPLE:_** float

Period is the type representing tick period

> **_EXAMPLE:_** std::milli is the period of one millisecond

> **_EXAMPLE:_** std::ratio<1, 1> is the period of one second

---

```c++
public:
explicit constexpr 
cu0::AsyncCoarseTimer<Rep, Period>::AsyncCoarseTimer(
    std::chrono::duration<Rep, Period> duration
);
```

constructs an instance with the specified duration

_Parameters_

duration is the duration which should be waited after launch

---

```c++
public:
constexpr void cu0::AsyncCoarseTimer<Rep, Period>::launch();
```

launches the timer

> **_SEE:_** cu0::AsyncCoarseTimer<Rep, Period>::wait()

---

```c++
public:
constexpr void cu0::AsyncCoarseTimer<Rep, Period>::wait() const;
```

waits for the timer to be up if it is not already

---

```c++
protected:
std::chrono::time_point<
    std::chrono::steady_clock,
    std::chrono::duration<Rep, Period>
> cu0::AsyncCoarseTimer<Rep, Period>::launchTime_;
```

time point when timer was launched

---

#### `cu0::BlockCoarseTimer`

---

```c++
template <class Rep, class Period>
struct cu0::BlockCoarseTimer;
```

struct representing blocking coarse timer which can be launched

_Template parameters_

Rep is the type representing number of ticks

> **_EXAMPLE:_** float

Period is the type representing tick period

> **_EXAMPLE:_** std::milli is the period of one millisecond

> **_EXAMPLE:_** std::ratio<1, 1> is the period of one second

---

```c++
public:
explicit constexpr 
cu0::BlockCoarseTimer<Rep, Period>::BlockCoarseTimer(
    std::chrono::duration<Rep, Period> duration
);
```

constructs an instance with the specified duration

_Parameters_

duration is the duration which should be waited after launch

---

```c++
public:
constexpr void cu0::BlockCoarseTimer<Rep, Period>::launch();
```

launches the timer and sleeps the specified duration

> **_SEE:_** cu0::BlockCoarseTimer<Rep, Period>::duration_

---

```c++
protected:
std::chrono::duration<Rep, Period> 
cu0::BlockCoarseTimer<Rep, Period>::duration_;
```

duration to sleep and block after launch

---
