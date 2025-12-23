#ifndef CU0_PROCESS_HH__
#define CU0_PROCESS_HH__

#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
    cu0::Process::current() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::create() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdin() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdin_cautious() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdout() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdout_cautious() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stderr() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stderr_cautious() will not be supported
#endif
#if !__has_include(<sys/types.h>)
#warning <sys/types.h> is not found => \
    cu0::Process::wait() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::wait_cautious() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::exit_code() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::termination_code() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::stop_code() will not be supported
#endif
#if !__has_include(<sys/wait.h>)
#warning <sys/wait.h> is not found => \
    cu0::Process::wait() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::wait_cautious() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::exit_code() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::termination_code() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::stop_code() will not be supported
#endif
#if !__has_include(<signal.h>)
#warning <signal.h> is not found => \
    cu0::Process::signal() will not be supported
#warning <signal.h> is not found => \
    cu0::Process::signal_cautious() will not be supported
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
   * @brief enum of possible errors for create() and create_pipeless() functions
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
   * @brief enum of possible errors for wait_cautious() function
   */
  enum struct WaitError {
    CHILD = ECHILD, //! @see ECHILD
    INVAL = EINVAL, //! @see EINVAL
    INTR = EINTR, //! @see EINTR
  };
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief enum of possible errors for stdin_cautious() function
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
   * @brief enum of possible errors for stdout_cautious() and stderr_cautious()
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
   * @brief enum of possible errors for signal_cautious() function
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
   *     stdin(), stdout(), stderr() and their corresponding cautious member
   *     functions are not supported yet
   *     @see implementation details of Process::current()
   * @return current process
   */
  [[nodiscard]]
  static Process current();
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief creates a process using the specified executable
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
   * @brief creates a process without pipes using the specified executable
   * @param executable is the excutable to be run by the process
   * @return
   *     if no error was reported => created process
   *     else => error code
   */
  [[nodiscard]]
  static std::variant<Process, CreateError> create_pipeless(
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
  constexpr std::optional<int> stdin_pipe() const;
  /*!
   * @brief accesses stdout pipe file descriptor
   * @return
   *     if stdout pipe file descriptor is valid => its value
   *     else => empty optional
   */
  [[nodiscard]]
  constexpr std::optional<int> stdout_pipe() const;
  /*!
   * @brief accesses stderr pipe file descriptor
   * @return
   *     if stderr pipe file descriptor is valid => its value
   *     else => empty optional
   */
  [[nodiscard]]
  constexpr std::optional<int> stderr_pipe() const;
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief waits for the process to exit or to be terminated or to be stopped
   */
  void wait();
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief waits for the process to exit or to be terminated or to be stopped
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  [[nodiscard]]
  std::variant<std::monostate, WaitError> wait_cautious();
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses exit status code
   * @note exit status code will be empty if the process hasn't exited normally
   *     @see Process::wait()
   * @return exit status code as a const reference
   */
  [[nodiscard]]
  constexpr const std::optional<int>& exit_code() const;
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses termination signal code
   * @note termination signal code will be empty if the process hasn't been
   *     terminated
   *     @see Process::wait()
   * @return termination signal code as a const reference
   */
  [[nodiscard]]
  constexpr const std::optional<int>& termination_code() const;
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses stop signal code
   * @note stop signal code will be empty if the process hasn't been stopped
   *     @see Process::wait()
   * @return stop signal code as a const reference
   */
  [[nodiscard]]
  constexpr const std::optional<int>& stop_code() const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief passes the specified input to the stdin
   * @param input is the input value
   */
  void stdin(const std::string& input) const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief passes the specified input to the stdin
   * @param input is the input value
   * @return result of Process::write_into() @see Process::write_into()
   */
  [[nodiscard]]
  std::tuple<std::variant<std::monostate, WriteError>, std::size_t>
      stdin_cautious(const std::string& input) const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief returns the value of the stdout
   * @return string containing stdout value
   */
  [[nodiscard]]
  std::string stdout() const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief returns the value of the stdout
   * @return result of Process::read_from() @see Process::read_from()
   */
  [[nodiscard]]
  std::tuple<std::variant<std::monostate, ReadError>, std::string>
      stdout_cautious() const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief returns the value of the stderr
   * @return string containing stderr value
   */
  [[nodiscard]]
  std::string stderr() const;
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief returns the value of the stderr
   * @return result of Process::read_from() @see Process::read_from()
   */
  [[nodiscard]]
  std::tuple<std::variant<std::monostate, ReadError>, std::string>
      stderr_cautious() const;
#endif
#if __has_include(<signal.h>)
  /*!
   * @brief sends the specified code as a signal to this process
   * @param code is the signal to be sent
   */
  void signal(const int& code) const;
#endif
#if __has_include(<signal.h>)
  /*!
   * @brief sends the specified code as a signal to this process
   * @param code is the signal to be sent
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  [[nodiscard]]
  std::variant<std::monostate, SignalError> signal_cautious(
      const int& code
  ) const;
#endif
protected:
#if __has_include(<unistd.h>)
  /*!
   * @brief writes the specified input into the specified pipe
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
  static Return write_into(
      const int& pipe,
      const std::string& input
  );
#endif
#if __has_include(<unistd.h>)
  /*!
   * @brief reads from the specified pipe
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
  static Return read_from(const int& pipe);
#endif
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief loops to wait for process exit
   * @tparam Return is the type to be returned by this function
   * @return
   *     if Return == std::variant<std::monostate, WaitError> =>
   *         if no error was reported => std::monostate
   *         else =>
   *             error code of the first encountered error
   *     if Return == void => no errors are returned and handled
   */
  template <class Return>
  Return wait_exit_loop();
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
  int stdin_pipe_ = -1;
  //! stdout file descriptor
  int stdout_pipe_ = -1;
  //! stderr file descriptor
  int stderr_pipe_ = -1;
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  //! if waited => actual exit status code value if present @see Process::wait()
  //! else => empty exit status code value
  std::optional<int> exit_code_ = {};
  //! if waited => actual termination signal code value if present
  //!     @see Process::wait()
  //! else => empty termination signal code value
  std::optional<int> termination_code_ = {};
  //! if waited => actual stop signal code value if present @see Process::wait()
  //! else => empty stop signal code value
  std::optional<int> stop_code_ = {};
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
  ret.stdin_pipe_ = -1;
  //! reading from stdout of the same process is not supported yet =>
  //!     set stdout pipe to -1
  //! there will be an empty string value if stdout() is tried to be called
  //! though some errors may be returned by a call
  ret.stdout_pipe_ = -1;
  //! reading stderr of the same process is not supported yet =>
  //!     set stderr pipe to -1
  //! there will be an empty string value if stderr() is tried to be called
  //! though some errors may be returned by a call
  ret.stderr_pipe_ = -1;
  //! return the process
  return ret;
}
#endif

#if __has_include(<unistd.h>)
inline std::variant<Process, typename Process::CreateError> Process::create(
    const Executable& executable
) {
  const auto [argv, argv_size] = util::argv_of(executable);
  const auto [envp, envp_size] = util::envp_of(executable);
  auto argv_raw = std::make_unique<char*[]>(argv_size);
  for (auto i = 0u; i < argv_size; i++) {
    argv_raw[i] = argv[i].get();
  }
  auto envp_raw = std::make_unique<char*[]>(envp_size);
  for (auto i = 0u; i < envp_size; i++) {
    envp_raw[i] = envp[i].get();
  }
  int in_fd[2];
  int out_fd[2];
  int err_fd[2];
  if (::pipe(in_fd) != 0) {
    return static_cast<CreateError>(errno);
  }
  if (::pipe(out_fd) != 0) {
    const auto ret = static_cast<CreateError>(errno);
    ::close(in_fd[0]);
    ::close(in_fd[1]);
    return ret;
  }
  if (::pipe(err_fd) != 0) {
    const auto ret = static_cast<CreateError>(errno);
    ::close(in_fd[0]);
    ::close(in_fd[1]);
    ::close(out_fd[0]);
    ::close(out_fd[1]);
    return ret;
  }
  const auto pid = ::vfork();
  if (pid == 0) { //! forked process
    ::close(in_fd[1]);
    ::close(out_fd[0]);
    ::close(err_fd[0]);
    ::dup2(in_fd[0], STDIN_FILENO);
    ::dup2(out_fd[1], STDOUT_FILENO);
    ::dup2(err_fd[1], STDERR_FILENO);
    ::close(in_fd[0]);
    ::close(out_fd[1]);
    ::close(err_fd[1]);
    const auto exec_ret = ::execve(argv_raw[0], argv_raw.get(), envp_raw.get());
    if (exec_ret != 0) {
      //! fail
      exit(errno);
    }
  }
  ::close(in_fd[0]);
  ::close(out_fd[1]);
  ::close(err_fd[1]);
  if (pid < 0) { //! fork failed
    return static_cast<CreateError>(errno);
  }
  auto process = Process{};
  process.pid_ = pid;
  process.stdin_pipe_ = in_fd[1];
  process.stdout_pipe_ = out_fd[0];
  process.stderr_pipe_ = err_fd[0];
  return process;
}
#endif

#if __has_include(<unistd.h>)
inline std::variant<Process, typename Process::CreateError>
Process::create_pipeless(
    const Executable& executable
) {
  const auto [argv, argv_size] = util::argv_of(executable);
  const auto [envp, envp_size] = util::envp_of(executable);
  auto argv_raw = std::make_unique<char*[]>(argv_size);
  for (auto i = 0u; i < argv_size; i++) {
    argv_raw[i] = argv[i].get();
  }
  auto envp_raw = std::make_unique<char*[]>(envp_size);
  for (auto i = 0u; i < envp_size; i++) {
    envp_raw[i] = envp[i].get();
  }
  const auto pid = ::vfork();
  if (pid == 0) { //! forked process
    const auto exec_ret = ::execve(argv_raw[0], argv_raw.get(), envp_raw.get());
    if (exec_ret != 0) {
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
  ::close(this->stdin_pipe_);
  ::close(this->stdout_pipe_);
  ::close(this->stderr_pipe_);
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

constexpr std::optional<int> Process::stdin_pipe() const {
  if (this->stdin_pipe_ < 0) {
    return {};
  }
  return this->stdin_pipe_;
}

constexpr std::optional<int> Process::stdout_pipe() const {
  if (this->stdout_pipe_ < 0) {
    return {};
  }
  return this->stdout_pipe_;
}

constexpr std::optional<int> Process::stderr_pipe() const {
  if (this->stderr_pipe_ < 0) {
    return {};
  }
  return this->stderr_pipe_;
}

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
inline void Process::wait() {
  this->wait_exit_loop<void>();
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
inline std::variant<std::monostate, typename Process::WaitError>
Process::wait_cautious() {
  return this->wait_exit_loop<std::variant<std::monostate, WaitError>>();
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
constexpr const std::optional<int>& Process::exit_code() const {
  return this->exit_code_;
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
constexpr const std::optional<int>& Process::termination_code() const {
  return this->termination_code_;
}
#endif

#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
constexpr const std::optional<int>& Process::stop_code() const {
  return this->stop_code_;
}
#endif

#if __has_include(<unistd.h>)
inline void Process::stdin(const std::string& input) const {
  return Process::write_into<1024, void>(this->stdin_pipe_, input);
}
#endif

#if __has_include(<unistd.h>)
inline std::tuple<
    std::variant<std::monostate, typename Process::WriteError>,
    std::size_t
> Process::stdin_cautious(
    const std::string& input
) const {
  return Process::write_into<
      1024,
      std::tuple<std::variant<std::monostate, WriteError>, std::size_t>
  >(this->stdin_pipe_, input);
}
#endif

#if __has_include(<unistd.h>)
inline std::string
Process::stdout() const {
  return Process::read_from<1024, std::string>(this->stdout_pipe_);
}
#endif

#if __has_include(<unistd.h>)
inline std::tuple<
    std::variant<std::monostate, typename Process::ReadError>,
    std::string
> Process::stdout_cautious() const {
  return Process::read_from<
      1024,
      std::tuple<std::variant<std::monostate, ReadError>, std::string>
  >(this->stdout_pipe_);
}
#endif

#if __has_include(<unistd.h>)
inline std::string Process::stderr() const {
  return Process::read_from<1024, std::string>(this->stderr_pipe_);
}
#endif

#if __has_include(<unistd.h>)
inline std::tuple<
    std::variant<std::monostate, typename Process::ReadError>,
    std::string
> Process::stderr_cautious() const {
  return Process::read_from<
      1024,
      std::tuple<std::variant<std::monostate, ReadError>, std::string>
  >(this->stderr_pipe_);
}
#endif

#if __has_include(<signal.h>)
inline void Process::signal(const int& code) const {
  ::kill(this->pid_, code);
}
#endif

#if __has_include(<signal.h>)
inline std::variant<std::monostate, typename Process::SignalError>
Process::signal_cautious(
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
Return Process::write_into(
    const int& pipe,
    const std::string& input
) {
  using non_void_return_type =
      std::tuple<std::variant<std::monostate, WriteError>, std::size_t>;
  static_assert(
      std::is_same_v<Return, void> ||
      std::is_same_v<Return, non_void_return_type>
  );
  [[maybe_unused]] std::size_t bytes_written;
  if constexpr (std::is_same_v<Return, non_void_return_type>) {
    bytes_written = std::size_t{0};
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
        auto write_result = ::write(pipe, buffer, end);
        ;
        write_result = ::write(pipe, buffer + bytes, end - bytes)
    ) {
      if (write_result < 0) {
        if constexpr (std::is_same_v<Return, void>) {
          return;
        } else { //! std::is_same_v<Return, non_void_return_type>
          bytes_written += bytes;
          return { static_cast<WriteError>(errno), bytes_written, };
        }
      }
      bytes += write_result;
      if (bytes >= static_cast<ssize_t>(end)) {
        break;
      }
    }
    if constexpr (std::is_same_v<Return, non_void_return_type>) {
      bytes_written += bytes;
    }
  }
  if constexpr (std::is_same_v<Return, void>) {
    return;
  } else { //! std::is_same_v<Return, non_void_return_type>
    return { std::monostate{}, bytes_written, };
  }
}
#endif

#if __has_include(<unistd.h>)
template <std::size_t BUFFER_SIZE, class Return>
inline Return Process::read_from(const int& pipe) {
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
inline Return Process::wait_exit_loop() {
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
        this->termination_code_ = WTERMSIG(status);
        //! no error handling is needed because the process has been waited
        //!     even if it was terminated
      }
      if (WIFSTOPPED(status) != 0) {
        this->stop_code_ = WSTOPSIG(status);
        //! no error handling is needed because the process has been waited
        //!     even if it was stopped
      }
      break;
    }
    this->exit_code_ = WEXITSTATUS(status);
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
  std::swap(this->stdin_pipe_, other.stdin_pipe_);
  std::swap(this->stdout_pipe_, other.stdout_pipe_);
  std::swap(this->stderr_pipe_, other.stderr_pipe_);
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  std::swap(this->exit_code_, other.exit_code_);
  std::swap(this->termination_code_, other.termination_code_);
  std::swap(this->stop_code_, other.stop_code_);
#endif
}

} /// namespace cu0

#endif /// CU0_PROCESS_HH__
