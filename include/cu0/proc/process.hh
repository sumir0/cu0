#ifndef CU0_PROCESS_HH_
#define CU0_PROCESS_HH_

#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
    cu0::Process::current() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::create() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdin() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdinCautious() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdout() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdoutCautious() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stderr() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stderrCautious() will not be supported
#endif
#if !__has_include(<sys/types.h>)
#warning <sys/types.h> is not found => \
    cu0::Process::wait() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::waitCautious() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::exitCode() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::terminationCode() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::stopCode() will not be supported
#endif
#if !__has_include(<sys/wait.h>)
#warning <sys/wait.h> is not found => \
    cu0::Process::wait() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::waitCautious() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::exitCode() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::terminationCode() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::stopCode() will not be supported
#endif
#if !__has_include(<signal.h>)
#warning <signal.h> is not found => \
    cu0::Process::signal() will not be supported
#warning <signal.h> is not found => \
    cu0::Process::signalCautious() will not be supported
#endif

#include <optional>
#include <sstream>
#include <variant>

#if __has_include(<unistd.h>)
#include <unistd.h>
#endif
#if __has_include(<sys/types.h>)
#include <sys/types.h>
#endif
#if __has_include(<sys/wait.h>)
#include <sys/wait.h>
#endif
#if __has_include(<signal.h>)
#include <signal.h>
#endif

#include <cu0/proc/executable.hh>

namespace cu0 {

/*!
 * @brief The Process struct provides a way to access process-specific data
 */
struct Process {
public:
#if __has_include(<unistd.h>)
  /*!
   * @brief enum of possible errors for create() and createPipeless() functions
   */
  enum struct CreateError {
    AGAIN = EAGAIN, //! @see EAGAIN
    NOMEM = ENOMEM, //! @see ENOMEM
    FAULT = EFAULT, //! @see EFAULT
    INVAL = EINVAL, //! @see EINVAL
    MFILE = EMFILE, //! @see EMFILE
    NFILE = ENFILE, //! @see ENFILE
  };
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief enum of possible errors for waitCautious() function
   */
  enum struct WaitError {
    CHILD = ECHILD, //! @see ECHILD
    INVAL = EINVAL, //! @see EINVAL
    INTR = EINTR, //! @see EINTR
  };
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief enum of possible errors for stdinCautious() function
   */
  enum struct WriteError {
    AGAIN = EAGAIN, //! @see EAGAIN
#if EWOULDBLOCK != EAGAIN
    WOULDBLOCK = EWOULDBLOCK, //! @see EWOULDBLOCK
#endif
    BADF = EBADF, //! @see EBADF
    DESTADDRREQ = EDESTADDRREQ, //! @see EDESTADDRREQ
    DQUOT = EDQUOT, //! @see EDQUOT
    FAULT = EFAULT, //! @see EFAULT
    FBIG = EFBIG, //! @see EFBIG
    INTR = EINTR, //! @see EINTR
    INVAL = EINVAL, //! @see EINVAL
    IO = EIO, //! @see EIO
    NOSPC = ENOSPC, //! @see ENOSPC
    PERM = EPERM, //! @see EPERM
    PIPE = EPIPE, //! @see EPIPE
    //! it is possible that a value is not listed in this enum =>
    //!     for other error codes @see ::write()
  };
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief enum of possible errors for stdoutCautious() and stderrCautious()
   *     functions
   */
  enum struct ReadError {
    AGAIN = EAGAIN, //! @see EAGAIN
    BADF = EBADF, //! @see EBADF
    BADMSG = EBADMSG, //! @see EBADMSG
    INTR = EINTR, //! @see EINTR
    INVAL = EINVAL, //! @see EINVAL
    IO = EIO, //! @see EIO
    ISDIR = EISDIR, //! @see EISDIR
    OVERFLOW = EOVERFLOW, //! @see EOVERFLOW
#if EWOULDBLOCK != EAGAIN
    WOULDBLOCK = EWOULDBLOCK, //! @see EWOULDBLOCK
#endif
    CONNRESET = ECONNRESET, //! @see ECONNRESET
    NOTCONN = ENOTCONN, //! @see ENOTCONN
    TIMEDOUT = ETIMEDOUT, //! @see ETIMEDOUT
    NOBUFS = ENOBUFS, //! @see ENOBUFS
    NOMEM = ENOMEM, //! @see ENOMEM
    NXIO = ENXIO, //! @see ENXIO
    //! it is possible that a value is not listed in this enum =>
    //!     for other error codes @see ::read()
  };
#endif
#if __has_include(<signal.h>)
  /*!
   * @brief enum of possible errors for signalCautious() function
   */
  enum struct SignalError {
    INVAL = EINVAL, //! @see EINVAL
    PERM = EPERM, //! @see EPERM
    SRCH = ESRCH, //! @see ESRCH
  };
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief constructs an instance using the current process in which
   *     this function is called
   * @note for the returned current process
   *     stdin(), stdout(), stderr() member functions are not supported yet
   *     @see implementation details of Process::current()
   * @return current process
   */
  [[nodiscard]]
  static Process current();
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief creates a process using a specified executable
   * @param executable is the excutable to be run by the process
   * @return
   *     if no error was reported => created process
   *     else => error code
   */
  [[nodiscard]]
  static std::variant<Process, CreateError> create(
      const Executable& executable
  );
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief creates a process using a specified executable without pipes
   * @param executable is the excutable to be run by the process
   * @return
   *     if no error was reported => created process
   *     else => error code
   */
  [[nodiscard]]
  static std::variant<Process, CreateError> createPipeless(
      const Executable& executable
  );
#endif
  /*!
   * @brief destructs an instance
   */
  virtual ~Process();
  constexpr Process(const Process& other) = delete;
  constexpr Process& operator =(const Process& other) = delete;
  /*!
   * @brief moves process resources to this process
   * @param other is the process for which resources need to be moved
   */
  constexpr Process(Process&& other);
  /*!
   * @brief moves process resources to this process
   * @param other is the process for which resources need to be moved
   * @return this process as a mutable reference
   */
  constexpr Process& operator =(Process&& other);
  /*!
   * @brief accesses process identifier value
   * @return process identifier as a const reference
   */
  [[nodiscard]]
  constexpr const unsigned& pid() const;
  /*!
   * @brief accesses stdin pipe file descriptor
   * @return
   *     if stdin pipe file descriptor is valid => its value
   *     else => empty optional
   */
  [[nodiscard]]
  constexpr std::optional<int> stdinPipe() const;
  /*!
   * @brief accesses stdin pipe file descriptor
   * @return
   *     if stdin pipe file descriptor is valid => its value
   *     else => empty optional
   */
  [[nodiscard]]
  constexpr std::optional<int> stdoutPipe() const;
  /*!
   * @brief accesses stdin pipe file descriptor
   * @return
   *     if stdin pipe file descriptor is valid => its value
   *     else => empty optional
   */
  [[nodiscard]]
  constexpr std::optional<int> stderrPipe() const;
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief waits for the process to exit or to be terminated or to be stopped
   */
  void wait();
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief waitCautious waits for the process to exit or to be terminated or
   *     to be stopped
   * @note error code equal to 0 indicates no error
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  [[nodiscard]]
  std::variant<std::monostate, WaitError> waitCautious();
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses exit status code
   * @note exit status code will be empty until the process has been waited
   *     @see Process::wait()
   * @return exit status code as a const reference
   */
  [[nodiscard]]
  constexpr const std::optional<int>& exitCode() const;
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses termination signal code
   * @note termination signal code will be empty until
   *     the process has been waited @see Process::wait()
   * @return termination signal code as a const reference
   */
  [[nodiscard]]
  constexpr const std::optional<int>& terminationCode() const;
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses stop signal code
   * @note stop signal code will be empty until
   *     the process has been waited @see Process::wait()
   * @return stop signal code as a const reference
   */
  [[nodiscard]]
  constexpr const std::optional<int>& stopCode() const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief stdin passes the specified input to the stdin
   * @param input is the input value
   */
  void stdin(const std::string& input) const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief stdin passes the specified input to the stdin
   * @param input is the input value
   * @return result of Process::writeInto() @see Process::writeInto()
   */
  [[nodiscard]]
  std::tuple<std::variant<std::monostate, WriteError>, std::size_t>
      stdinCautious(const std::string& input) const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief stdout returns the value of the stdout
   * @return string containing stdout value
   */
  [[nodiscard]]
  std::string stdout() const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief stdout returns the value of the stdout
   * @return result of Process::readFrom() @see Process::readFrom()
   */
  [[nodiscard]]
  std::tuple<std::variant<std::monostate, ReadError>, std::string>
      stdoutCautious() const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief stderr returns the value of the stderr
   * @return string containing stderr value
   */
  [[nodiscard]]
  std::string stderr() const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief stderr returns the value of the stderr
   * @return result of Process::readFrom() @see Process::readFrom()
   */
  [[nodiscard]]
  std::tuple<std::variant<std::monostate, ReadError>, std::string>
      stderrCautious() const;
#endif
#if __has_include(<signal.h>)
  /*!
   * @brief signal sends a specified code as a signal to this process
   * @param code is the signal to be sent
   */
  void signal(const int& code) const;
#endif
#if __has_include(<signal.h>)
  /*!
   * @brief signal sends a specified code as a signal to this process
   * @param code is the signal to be sent
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  [[nodiscard]]
  std::variant<std::monostate, SignalError> signalCautious(
      const int& code
  ) const;
#endif
protected:
#if __has_include(<unistd.h>)
  /*!
   * @brief writeInto writes the specified input into the specified pipe
   * @tparam BUFFER_SIZE is the buffer size for writing into the pipe
   * @tparam Return is the type to be returned by this function
   * @param pipe is the pipe to write into
   * @param input is the data to write
   * @return
   *     if Return == std::tuple<
   *         std::variant<std::monostate, WriteError>,
   *         std::size_t
   *     > =>
   *         tuple containing
   *             variant of
   *                 if no error was reported => std::monostate
   *                 else => error code
   *             number of bytes written
   *                 if no error was reported =>
   *                     equal to input size
   *                 else =>
   *                     equal to the number of bytes that had already been
   *                     written before an error was reported
   *     if Return == void => nothing
   */
  template <std::size_t BUFFER_SIZE, class Return>
  static Return writeInto(
      const int& pipe,
      const std::string& input
  );
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief readFrom reads from the specified pipe
   * @tparam BUFFER_SIZE is the buffer size for reading from the pipe
   * @tparam Return is the type to be returned by this function
   * @param pipe is the pipe to read from
   * @return
   *     if Return == std::tuple<
   *         std::variant<std::monostate, ReadError>,
   *         std::string
   *     > =>
   *         tuple containing
   *             variant of
   *                 if no error was reported => std::monostate
   *                 else => error code
   *             data that have been read
   *                 if no error was reported => fully received
   *                 else =>
   *                     partially and contains data that had already been
   *                     read before an error was reported
   *     if Return == std::string => data that have been read as std::string
   */
  template <std::size_t BUFFER_SIZE, class Return>
  [[nodiscard]]
  static Return readFrom(const int& pipe);
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief loop to wait for process exit
   * @tparam Return is the type to be returned by this function
   * @return
   *     if Return == std::variant<std::monostate, WaitError> =>
   *         if no error was reported => std::monostate
   *         else =>
   *             error code of the first encountered error
   *     if Return == void => no errors are returned and handled
   */
  template <class Return>
  Return waitExitLoop();
#endif
  /*!
   * @brief constructs an instance with default values
   */
  constexpr Process() = default;
  /*!
   * @brief swaps two processes
   * @param other is the process to swap this process with
   */
  constexpr void swap(Process&& other);
  //! process identifier
  unsigned pid_ = 0;
  //! stdin file descriptor
  int stdinPipe_ = -1;
  //! stdout file descriptor
  int stdoutPipe_ = -1;
  //! stderr file descriptor
  int stderrPipe_ = -1;
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  //! if waited => actual exit status code value if present @see Process::wait()
  //! else => empty exit status code value
  std::optional<int> exitCode_ = {};
  //! if waited => actual termination signal code value if present
  //!     @see Process::wait()
  //! else => empty termination signal code value
  std::optional<int> terminationCode_ = {};
  //! if waited => actual stop signal code value if present @see Process::wait()
  //! else => empty stop signal code value
  std::optional<int> stopCode_ = {};
#endif
private:
};

} /// namespace cu0

namespace cu0 {

#if __has_include(<unistd.h>)
inline Process Process::current() {
  //! construct a process with default values
  auto ret = Process{};
  //! set process identifier
  ret.pid_ = getpid();
  //! writing to stdin of the same process is not supported yet =>
  //!     set stdin pipe to -1
  //! there will be no effect if stdin() is tried to be called
  //! though some errors may be returned by a call
  ret.stdinPipe_ = -1;
  //! reading from stdout of the same process is not supported yet =>
  //!     set stdout pipe to -1
  //! there will be an empty string value if stdout() is tried to be called
  //! though some errors may be returned by a call
  ret.stdoutPipe_ = -1;
  //! reading stderr of the same process is not supported yet =>
  //!     set stderr pipe to -1
  //! there will be an empty string value if stderr() is tried to be called
  //! though some errors may be returned by a call
  ret.stderrPipe_ = -1;
  //! return the process
  return ret;
}
#endif

#if __has_include(<unistd.h>)
inline std::variant<Process, typename Process::CreateError> Process::create(
    const Executable& executable
) {
  const auto [argv, argvSize] = util::argvOf(executable);
  const auto [envp, envpSize] = util::envpOf(executable);
  auto argvRaw = std::make_unique<char*[]>(argvSize);
  for (auto i = 0u; i < argvSize; i++) {
    argvRaw[i] = argv[i].get();
  }
  auto envpRaw = std::make_unique<char*[]>(envpSize);
  for (auto i = 0u; i < envpSize; i++) {
    envpRaw[i] = envp[i].get();
  }
  int inFd[2];
  int outFd[2];
  int errFd[2];
  if (::pipe(inFd) != 0) {
    return static_cast<CreateError>(errno);
  }
  if (::pipe(outFd) != 0) {
    const auto ret = static_cast<CreateError>(errno);
    ::close(inFd[0]);
    ::close(inFd[1]);
    return ret;
  }
  if (::pipe(errFd) != 0) {
    const auto ret = static_cast<CreateError>(errno);
    ::close(inFd[0]);
    ::close(inFd[1]);
    ::close(outFd[0]);
    ::close(outFd[1]);
    return ret;
  }
  const auto pid = ::vfork();
  if (pid == 0) { //! forked process
    ::close(inFd[1]);
    ::close(outFd[0]);
    ::close(errFd[0]);
    ::dup2(inFd[0], STDIN_FILENO);
    ::dup2(outFd[1], STDOUT_FILENO);
    ::dup2(errFd[1], STDERR_FILENO);
    ::close(inFd[0]);
    ::close(outFd[1]);
    ::close(errFd[1]);
    const auto execRet = ::execve(argvRaw[0], argvRaw.get(), envpRaw.get());
    if (execRet != 0) {
      //! fail
      exit(errno);
    }
  }
  ::close(inFd[0]);
  ::close(outFd[1]);
  ::close(errFd[1]);
  if (pid < 0) { //! fork failed
    return static_cast<CreateError>(errno);
  }
  auto process = Process{};
  process.pid_ = pid;
  process.stdinPipe_ = inFd[1];
  process.stdoutPipe_ = outFd[0];
  process.stderrPipe_ = errFd[0];
  return process;
}
#endif

#if __has_include(<unistd.h>)
inline std::variant<Process, typename Process::CreateError>
Process::createPipeless(
    const Executable& executable
) {
  const auto [argv, argvSize] = util::argvOf(executable);
  const auto [envp, envpSize] = util::envpOf(executable);
  auto argvRaw = std::make_unique<char*[]>(argvSize);
  for (auto i = 0u; i < argvSize; i++) {
    argvRaw[i] = argv[i].get();
  }
  auto envpRaw = std::make_unique<char*[]>(envpSize);
  for (auto i = 0u; i < envpSize; i++) {
    envpRaw[i] = envp[i].get();
  }
  const auto pid = ::vfork();
  if (pid == 0) { //! forked process
    const auto execRet = ::execve(argvRaw[0], argvRaw.get(), envpRaw.get());
    if (execRet != 0) {
      //! fail
      exit(errno);
    }
  }
  if (pid < 0) { //! fork failed
    return static_cast<CreateError>(errno);
  }
  auto process = Process{};
  process.pid_ = pid;
  return process;
}
#endif

inline Process::~Process() {
#if __has_include(<unistd.h>)
  ::close(this->stdinPipe_);
  ::close(this->stdoutPipe_);
  ::close(this->stderrPipe_);
#endif
}

constexpr Process::Process(Process&& other) {
  this->swap(std::move(other));
}

constexpr Process& Process::operator =(Process&& other) {
  if (this != &other) {
    this->swap(std::move(other));
  }
  return *this;
}

constexpr const unsigned& Process::pid() const {
  return this->pid_;
}

constexpr std::optional<int> Process::stdinPipe() const {
  if (this->stdinPipe_ < 0) {
    return {};
  }
  return this->stdinPipe_;
}

constexpr std::optional<int> Process::stdoutPipe() const {
  if (this->stdoutPipe_ < 0) {
    return {};
  }
  return this->stdoutPipe_;
}

constexpr std::optional<int> Process::stderrPipe() const {
  if (this->stderrPipe_ < 0) {
    return {};
  }
  return this->stderrPipe_;
}

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
inline void Process::wait() {
  this->waitExitLoop<void>();
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
inline std::variant<std::monostate, typename Process::WaitError>
Process::waitCautious() {
  return this->waitExitLoop<std::variant<std::monostate, WaitError>>();
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
constexpr const std::optional<int>& Process::exitCode() const {
  return this->exitCode_;
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
constexpr const std::optional<int>& Process::terminationCode() const {
  return this->terminationCode_;
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
constexpr const std::optional<int>& Process::stopCode() const {
  return this->stopCode_;
}
#endif

#if __has_include(<unistd.h>)
inline void Process::stdin(const std::string& input) const {
  return Process::writeInto<1024, void>(this->stdinPipe_, input);
}
#endif

#if __has_include(<unistd.h>)
inline std::tuple<
    std::variant<std::monostate, typename Process::WriteError>,
    std::size_t
> Process::stdinCautious(
    const std::string& input
) const {
  return Process::writeInto<
      1024,
      std::tuple<std::variant<std::monostate, WriteError>, std::size_t>
  >(this->stdinPipe_, input);
}
#endif

#if __has_include(<unistd.h>)
inline std::string
Process::stdout() const {
  return Process::readFrom<1024, std::string>(this->stdoutPipe_);
}
#endif

#if __has_include(<unistd.h>)
inline std::tuple<
    std::variant<std::monostate, typename Process::ReadError>,
    std::string
> Process::stdoutCautious() const {
  return Process::readFrom<
      1024,
      std::tuple<std::variant<std::monostate, ReadError>, std::string>
  >(this->stdoutPipe_);
}
#endif

#if __has_include(<unistd.h>)
inline std::string Process::stderr() const {
  return Process::readFrom<1024, std::string>(this->stderrPipe_);
}
#endif

#if __has_include(<unistd.h>)
inline std::tuple<
    std::variant<std::monostate, typename Process::ReadError>,
    std::string
> Process::stderrCautious() const {
  return Process::readFrom<
      1024,
      std::tuple<std::variant<std::monostate, ReadError>, std::string>
  >(this->stderrPipe_);
}
#endif

#if __has_include(<signal.h>)
inline void Process::signal(const int& code) const {
  ::kill(this->pid_, code);
}
#endif

#if __has_include(<signal.h>)
inline std::variant<std::monostate, typename Process::SignalError>
Process::signalCautious(
    const int& code
) const {
  if (::kill(this->pid_, code) != 0) {
    return static_cast<SignalError>(errno);
  }
  return std::monostate{};
}
#endif

#if __has_include(<unistd.h>)
template <std::size_t BUFFER_SIZE, class Return>
Return Process::writeInto(
    const int& pipe,
    const std::string& input
) {
  using non_void_return_type =
      std::tuple<std::variant<std::monostate, WriteError>, std::size_t>;
  static_assert(
      std::is_same_v<Return, void> ||
      std::is_same_v<Return, non_void_return_type>
  );
  [[maybe_unused]] std::size_t bytesWritten;
  if constexpr (std::is_same_v<Return, non_void_return_type>) {
    bytesWritten = std::size_t{0};
  }
  char buffer[BUFFER_SIZE];
  for (auto i = 0u; i <= input.size() / BUFFER_SIZE; i++) {
    const auto data = input.substr(i * BUFFER_SIZE, BUFFER_SIZE);
    std::size_t end;
    if (data.size() != BUFFER_SIZE) {
      end = data.size();
    } else {
      end = BUFFER_SIZE;
    }
    for (auto j = 0u; j < end; j++) {
      buffer[j] = data[j];
    }
    auto bytes = 0;
    for (
        auto writeResult = ::write(pipe, buffer, end);
        ;
        writeResult = ::write(pipe, buffer + bytes, end - bytes)
    ) {
      if (writeResult < 0) {
        if constexpr (std::is_same_v<Return, void>) {
          return;
        } else { //! std::is_same_v<Return, non_void_return_type>
          bytesWritten += bytes;
          return { static_cast<WriteError>(errno), bytesWritten, };
        }
      }
      bytes += writeResult;
      if (bytes >= static_cast<ssize_t>(end)) {
        break;
      }
    }
    if constexpr (std::is_same_v<Return, non_void_return_type>) {
      bytesWritten += bytes;
    }
  }
  if constexpr (std::is_same_v<Return, void>) {
    return;
  } else { //! std::is_same_v<Return, non_void_return_type>
    return { std::monostate{}, bytesWritten, };
  }
}
#endif

#if __has_include(<unistd.h>)
template <std::size_t BUFFER_SIZE, class Return>
inline Return Process::readFrom(const int& pipe) {
  using non_void_return_type =
      std::tuple<std::variant<std::monostate, ReadError>, std::string>;
  static_assert(
      std::is_same_v<Return, std::string> ||
      std::is_same_v<Return, non_void_return_type>
  );
  auto oss = std::ostringstream{};
  ssize_t bytes;
  do {
    char buffer[BUFFER_SIZE];
    static_assert(BUFFER_SIZE > 1, "BUFFER_SIZE needs to have space for '\\0'");
    bytes = ::read(pipe, buffer, BUFFER_SIZE - 1);
    if (bytes < 0) { //! read failed
      if constexpr (std::is_same_v<Return, std::string>) {
        return oss.str();
      } else { //! std::is_same_v<Return, non_void_return_type>
        return { static_cast<ReadError>(errno), oss.str(), };
      }
    }
    buffer[bytes] = '\0';
    oss << std::move(buffer);
  } while (bytes == BUFFER_SIZE - 1);
  if constexpr (std::is_same_v<Return, std::string>) {
    return oss.str();
  } else { //! std::is_same_v<Return, non_void_return_type>
    return { std::monostate{}, oss.str(), };
  }
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
template <class Return>
inline Return Process::waitExitLoop() {
  using non_void_return_type = std::variant<std::monostate, WaitError>;
  static_assert(
      std::is_same_v<Return, void> ||
      std::is_same_v<Return, non_void_return_type>
  );
  int status;
  while (true) {
    auto pid = ::waitpid(this->pid_, &status, WNOHANG);
    if (pid == 0) {
      continue;
    }
    if (pid == -1) {
      if constexpr (std::is_same_v<Return, non_void_return_type>) {
        return static_cast<WaitError>(errno);
      } else { //! std::is_same_v<Return, void>
        //! no error handling
        return;
      }
    }
    if (WIFEXITED(status) == 0) {
      if (WIFSIGNALED(status) != 0) {
        this->terminationCode_ = WTERMSIG(status);
        //! no error handling is needed because the process has been waited
        //!     even if it was terminated
      }
      if (WIFSTOPPED(status) != 0) {
        this->stopCode_ = WSTOPSIG(status);
        //! no error handling is needed because the process has been waited
        //!     even if it was stopped
      }
      break;
    }
    this->exitCode_ = WEXITSTATUS(status);
    break;
  }
  if constexpr (std::is_same_v<Return, non_void_return_type>) {
    return std::monostate{};
  } else { //! std::is_same_v<Return, void>
    return;
  }
}
#endif

constexpr void Process::swap(Process&& other) {
  std::swap(this->pid_, other.pid_);
  std::swap(this->stdinPipe_, other.stdinPipe_);
  std::swap(this->stdoutPipe_, other.stdoutPipe_);
  std::swap(this->stderrPipe_, other.stderrPipe_);
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  std::swap(this->exitCode_, other.exitCode_);
  std::swap(this->terminationCode_, other.terminationCode_);
  std::swap(this->stopCode_, other.stopCode_);
#endif
}

} /// namespace cu0

#endif /// CU0_PROCESS_HH_
