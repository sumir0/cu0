#ifndef CU0_PROCESS_HH_
#define CU0_PROCESS_HH_

#include <optional>
#include <sstream>
#include <variant>

#include <cu0/proc/executable.hh>

/*!
 * @brief checks software compatibility during compile-time
 */
#ifdef __unix__
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
#else
#include <unistd.h>
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
#else
#include <sys/types.h>
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
#else
#include <sys/wait.h>
#endif
#if !__has_include(<signal.h>)
#warning <signal.h> is not found => \
    cu0::Process::signal() will not be supported
#warning <signal.h> is not found => \
    cu0::Process::signalCautious() will not be supported
#else
#include <signal.h>
#endif
#else
#warning __unix__ is not defined => \
    cu0::Process::current() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::create() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::wait() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::waitCautious() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::exitCode() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::terminationCode() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::stdin() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::stdinCautious() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::stdout() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::stdoutCautious() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::stderr() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::stderrCautious() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::signal() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::signalCautious() will not be supported
#endif

namespace cu0 {

/*!
 * @brief The Process struct provides a way to access process-specific data
 */
struct Process {
public:
#ifdef __unix__
#if __has_include(<unistd.h>)
  enum struct CreateError {
    NO_ERROR = 0, //! no error
    AGAIN = EAGAIN, //! @see EAGAIN
    NOMEM = ENOMEM, //! @see ENOMEM
    FAULT = EFAULT, //! @see EFAULT
    INVAL = EINVAL, //! @see EINVAL
    MFILE = EMFILE, //! @see EMFILE
    NFILE = ENFILE, //! @see ENFILE
  };
#endif
#endif
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  enum struct WaitError {
    NO_ERROR = 0, //! no error
    CHILD = ECHILD, //! @see ECHILD
    INVAL = EINVAL, //! @see EINVAL
    INTR = EINTR, //! @see EINTR
  };
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  enum struct WriteError {
    NO_ERROR = 0, //! no error
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
    //! it is possible that a value is not listed in this enum ->
    //!     for other error codes @see ::write()
  };
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  enum struct ReadError {
    NO_ERROR = 0, //! no error
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
    //! it is possible that a value is not listed in this enum ->
    //!     for other error codes @see ::read()
  };
#endif
#endif
#ifdef __unix__
#if __has_include(<signal.h>)
  enum struct SignalError {
    NO_ERROR = 0, //! no error
    INVAL = EINVAL, //! @see EINVAL
    PERM = EPERM, //! @see EPERM
    SRCH = ESRCH, //! @see ESRCH
  };
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief constructs an instance using the current process in which
   *     this function is called
   * @note for the returned current process
   *     stdin(), stdout(), stderr() member functions are not supported yet
   *     @see implementation details of Process::current()
   * @return current process
   */
  [[nodiscard]] static Process current();
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief creates a process using the specified executable
   * @param executable is the excutable to be run by the process
   * @return
   *     if there were no errors -> created process
   *     if there was an error -> error code
   */
  [[nodiscard]] static std::variant<Process, CreateError> create(
      const Executable& executable
  );
#endif
#endif
  /*!
   * @brief destructs an instance
   */
  virtual ~Process();
  constexpr Process(const Process& other) = delete;
  constexpr Process& operator =(const Process& other) = delete;
  /*!
   * @brief moves the specified process resources to this process
   * @param other is the process to be moved
   */
  constexpr Process(Process&& other);
  /*!
   * @brief moves the specified process resources to this process
   * @param other is the process to be moved
   * @return this process as mutable reference
   */
  constexpr Process& operator =(Process&& other);
  /*!
   * @brief accesses process identifier value
   * @return process identifier as a const reference
   */
  constexpr const unsigned& pid() const;
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief waits for the process to exit or to be terminated or to be stopped
   */
  void wait();
#endif
#endif
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief waitCautious waits for the process to exit or to be terminated or
   *     to be stopped
   * @note error code equal to 0 indicates no error
   * @return error code @see WaitError
   */
  WaitError waitCautious();
#endif
#endif
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses exit status code
   * @note exit status code will be empty until the process has been waited
   *     @see Process::wait()
   * @return const reference to exit status code
   */
  constexpr const std::optional<int>& exitCode() const;
#endif
#endif
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses termination signal code
   * @note termination signal code will be empty until
   *     the process has been waited @see Process::wait()
   * @return const reference to termination signal code
   */
  constexpr const std::optional<int>& terminationCode() const;
#endif
#endif
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses stop signal code
   * @note stop signal code will be empty until
   *     the process has been waited @see Process::wait()
   * @return const reference to stop signal code
   */
  constexpr const std::optional<int>& stopCode() const;
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief stdin passes the specified input to the stdin
   * @param input is the input value
   */
  void stdin(const std::string& input) const;
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief stdin passes the specified input to the stdin
   * @param input is the input value
   * @return result of Process::writeInto() @see Process::writeInto()
   */
  std::tuple<WriteError, std::size_t> stdinCautious(
      const std::string& input
  ) const;
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief stdout returns the value of the stdout
   * @return string containing stdout value
   */
  std::string stdout() const;
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief stdout returns the value of the stdout
   * @return result of Process::readFrom() @see Process::readFrom()
   */
  std::tuple<std::string, ReadError> stdoutCautious() const;
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief stderr returns the value of the stderr
   * @return string containing stderr value
   */
  std::string stderr() const;
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief stderr returns the value of the stderr
   * @return result of Process::readFrom() @see Process::readFrom()
   */
  std::tuple<std::string, ReadError> stderrCautious() const;
#endif
#endif
#ifdef __unix__
#if __has_include(<signal.h>)
  /*!
   * @brief signal sends the specified code as a signal to the process
   * @param code is the signal to be sent
   */
  void signal(const int& code) const;
#endif
#endif
#ifdef __unix__
#if __has_include(<signal.h>)
  /*!
   * @brief signal sends the specified code as a signal to the process
   * @param code is the signal to be sent
   * @return
   *     if there were no errors -> SignalError::NO_ERROR
   *     if there was an error -> error code (not SignalError::NO_ERROR)
   */
  SignalError signalCautious(const int& code) const;
#endif
#endif
protected:
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief writeInto writes the specified input into the specified pipe
   * @tparam BUFFER_SIZE is the buffer size for writing into the pipe
   * @tparam Return is the type to be returned by this function
   * @param pipe is the pipe to write into
   * @param input is the data to write
   * @return
   *     if Return == std::tuple<WriteError, std::size_t> ->
   *         tuple containing
   *             error code
   *                 if there were no errors -> WriteError::NO_ERROR
   *                 if there was an error -> errno (not WriteError::NO_ERROR)
   *             number of bytes written
   *                 if there were no errors -> it should be equal to input size
   *                 if there was an error -> it should be equal to
   *                     the number of bytes that had already
   *                     been written before an error occured
   *     if Return == void -> nothing
   */
  template <std::size_t BUFFER_SIZE, class Return>
  static Return writeInto(
      const int& pipe,
      const std::string& input
  );
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief readFrom reads from the specified pipe
   * @tparam BUFFER_SIZE is the buffer size for reading from the pipe
   * @tparam Return is the type to be returned by this function
   * @param pipe is the pipe to read from
   * @return
   *     if Return == std::tuple<std::string, ReadError> ->
   *         tuple containing read value and error code
   *         if there were no errors ->
   *             std::string contains read value
   *             ReadError is equal to ReadError::NO_ERROR
   *         if there was an error ->
   *             std::string contains value that had been already read before
   *                 error occured
   *             ReadError is equal to error code
   *     if Return == std::string -> read value as std::string
   */
  template <std::size_t BUFFER_SIZE, class Return>
  static Return readFrom(const int& pipe);
#endif
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
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief loop to wait for process exit
   * @tparam Return is the return type
   *     if Return == void -> no errors are returned and handled
   *     else -> the first encountered error is returned
   */
  template <class Return>
  Return waitExitLoop();
#endif
#endif
  //! process identifier
  unsigned pid_ = 0;
  //! stdin file descriptor
  int stdinPipe_ = -1;
  //! stdout file descriptor
  int stdoutPipe_ = -1;
  //! stderr file descriptor
  int stderrPipe_ = -1;
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  //! if waited -> actual exit status code value if present @see Process::wait()
  //! else -> empty exit status code value
  std::optional<int> exitCode_ = {};
  //! if waited -> actual termination signal code value if present
  //!     @see Process::wait()
  //! else -> empty termination signal code value
  std::optional<int> terminationCode_ = {};
  //! if waited -> actual stop signal code value if present @see Process::wait()
  //! else -> empty stop signal code value
  std::optional<int> stopCode_ = {};
#endif
#endif
private:
};

} /// namespace cu0

namespace cu0 {

#ifdef __unix__
#if __has_include(<unistd.h>)
inline Process Process::current() {
  //! construct a process with default values
  auto ret = Process{};
  //! set process identifier
  ret.pid_ = getpid();
  //! writing to stdin of the same process is not supported yet ->
  //!     set stdin pipe to -1
  //! there will be no effect if stdin() is tried to be called
  //! though some errors may be returned by a call
  ret.stdinPipe_ = -1;
  //! reading from stdout of the same process is not supported yet ->
  //!     set stdout pipe to -1
  //! there will be an empty string value if stdout() is tried to be called
  //! though some errors may be returned by a call
  ret.stdoutPipe_ = -1;
  //! reading stderr of the same process is not supported yet ->
  //!     set stderr pipe to -1
  //! there will be an empty string value if stderr() is tried to be called
  //! though some errors may be returned by a call
  ret.stderrPipe_ = -1;
  //! return the process
  return ret;
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::variant<Process, Process::CreateError> Process::create(
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
#endif

inline Process::~Process() {
#ifdef __unix__
#if __has_include(<unistd.h>)
  ::close(this->stdinPipe_);
  ::close(this->stdoutPipe_);
  ::close(this->stderrPipe_);
#endif
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

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
inline void Process::wait() {
  this->waitExitLoop<void>();
}
#endif
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
inline typename Process::WaitError Process::waitCautious() {
  return this->waitExitLoop<WaitError>();
}
#endif
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
constexpr const std::optional<int>& Process::exitCode() const {
  return this->exitCode_;
}
#endif
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
constexpr const std::optional<int>& Process::terminationCode() const {
  return this->terminationCode_;
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline void Process::stdin(const std::string& input) const {
  return Process::writeInto<1024, void>(this->stdinPipe_, input);
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::tuple<typename Process::WriteError, std::size_t>
Process::stdinCautious(
    const std::string& input
) const {
  return Process::writeInto<1024, std::tuple<WriteError, std::size_t>>(
      this->stdinPipe_, input
  );
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::string
Process::stdout() const {
  return Process::readFrom<1024, std::string>(this->stdoutPipe_);
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::tuple<std::string, typename Process::ReadError>
Process::stdoutCautious() const {
  return Process::readFrom<1024, std::tuple<std::string, ReadError>>(
      this->stdoutPipe_
  );
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::string Process::stderr() const {
  return Process::readFrom<1024, std::string>(this->stderrPipe_);
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::tuple<std::string, typename Process::ReadError>
Process::stderrCautious() const {
  return Process::readFrom<1024, std::tuple<std::string, ReadError>>(
      this->stderrPipe_
  );
}
#endif
#endif

#ifdef __unix__
#if __has_include(<signal.h>)
inline void Process::signal(const int& code) const {
  ::kill(this->pid_, code);
}
#endif
#endif

#ifdef __unix__
#if __has_include(<signal.h>)
inline typename Process::SignalError Process::signalCautious(
    const int& code
) const {
  if (::kill(this->pid_, code) != 0) {
    return static_cast<SignalError>(errno);
  }
  return SignalError::NO_ERROR;
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
template <std::size_t BUFFER_SIZE, class Return>
Return Process::writeInto(
    const int& pipe,
    const std::string& input
) {
  static_assert(
      std::is_same_v<Return, void> ||
      std::is_same_v<Return, std::tuple<WriteError, std::size_t>>
  );
  [[maybe_unused]] std::size_t bytesWritten;
  if constexpr (std::is_same_v<Return, std::tuple<WriteError, std::size_t>>) {
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
        } else { //! std::is_same_v<Return, std::tuple<WriteError, std::size_t>>
          bytesWritten += bytes;
          return { static_cast<WriteError>(errno), bytesWritten, };
        }
      }
      bytes += writeResult;
      if (bytes >= static_cast<ssize_t>(end)) {
        break;
      }
    }
    if constexpr (std::is_same_v<Return, std::tuple<WriteError, std::size_t>>) {
      bytesWritten += bytes;
    }
  }
  if constexpr (std::is_same_v<Return, void>) {
    return;
  } else { //! std::is_same_v<Return, std::tuple<WriteError, std::size_t>>
    return { WriteError::NO_ERROR, bytesWritten, };
  }
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
template <std::size_t BUFFER_SIZE, class Return>
inline Return Process::readFrom(const int& pipe) {
  static_assert(
      std::is_same_v<Return, std::string> ||
      std::is_same_v<Return, std::tuple<std::string, ReadError>>
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
      } else { //! std::is_same_v<Return, std::tuple<std::string, ReadError>>
        return { oss.str(), static_cast<ReadError>(errno), };
      }
    }
    buffer[bytes] = '\0';
    oss << std::move(buffer);
  } while (bytes == BUFFER_SIZE - 1);
  if constexpr (std::is_same_v<Return, std::string>) {
    return oss.str();
  } else { //! std::is_same_v<Return, std::tuple<std::string, ReadError>>
    return { oss.str(), ReadError::NO_ERROR, };
  }
}
#endif
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
template <class Return>
inline Return Process::waitExitLoop() {
  static_assert(
      std::is_same_v<Return, void> ||
      std::is_same_v<Return, WaitError>
  );
  int status;
  while (true) {
    auto pid = ::waitpid(this->pid_, &status, WNOHANG);
    if (pid == 0) {
      continue;
    }
    if (pid == -1) {
      if constexpr (std::is_same_v<Return, WaitError>) {
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
  if constexpr (std::is_same_v<Return, WaitError>) {
    return WaitError::NO_ERROR;
  } else { //! std::is_same_v<Return, void>
    return;
  }
}
#endif
#endif

constexpr void Process::swap(Process&& other) {
  std::swap(this->pid_, other.pid_);
  std::swap(this->stdinPipe_, other.stdinPipe_);
  std::swap(this->stdoutPipe_, other.stdoutPipe_);
  std::swap(this->stderrPipe_, other.stderrPipe_);
  std::swap(this->exitCode_, other.exitCode_);
}

} /// namespace cu0

#endif /// CU0_PROCESS_HH_
