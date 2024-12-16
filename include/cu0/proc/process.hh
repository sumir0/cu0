#ifndef CU0_PROCESS_HH_
#define CU0_PROCESS_HH_

#include <optional>
#include <sstream>

#include <cu0/proc/executable.hh>

#include <iostream>

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
    cu0::Process::stdout() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stderr() will not be supported
#else
#include <unistd.h>
#endif
#if !__has_include(<sys/types.h>)
#warning <sys/types.h> is not found => \
    cu0::Process::wait() will not be supported
#warning <sys/types.h> is not found => \
    cu0::Process::exitCode() will not be supported
#else
#include <sys/types.h>
#endif
#if !__has_include(<sys/wait.h>)
#warning <sys/types.h> is not found => \
    cu0::Process::wait() will not be supported
#warning <sys/wait.h> is not found => \
    cu0::Process::exitCode() will not be supported
#else
#include <sys/wait.h>
#endif
#else
#warning __unix__ is not defined => \
    cu0::Process::current() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::create() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::wait() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::exitCode() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdin() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stdout() will not be supported
#warning <unistd.h> is not found => \
    cu0::Process::stderr() will not be supported
#endif

namespace cu0 {

/*!
 * @brief The Process struct provides a way to access process-specific data
 */
struct Process {
public:
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief constructs an instance using the current process in which
   *     this function is called
   * @note for the returned current process
   *     stdin(), stdout(), stderr() member functions are not supported yet
   *     @see implementation details
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
   * @return created process
   */
  [[nodiscard]] static std::optional<Process> create(
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
   * @brief waits for a process to exit
   * @return mutable process
   */
  Process& wait();
#endif
#endif
#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
  /*!
   * @brief accesses exit status code
   * @note exit status code will be empty until the process has been waited for
   *     @see Process::wait()
   * @return copy of exit status code
   */
  constexpr const std::optional<int>& exitCode() const;
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
   * @brief stdout returns the value of the stdout
   * @return stdout as a std::string
   */
  std::string stdout() const;
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief stderr returns the value of the stderr
   * @return stderr as a std::string
   */
  std::string stderr() const;
#endif
#endif
protected:
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief writeInto writes the specified input into the specified pipe
   * @tparam BUFFER_SIZE is the buffer size for writing into the pipe
   * @param pipe is the pipe to write into
   * @param input is the data to write
   */
  template <std::size_t BUFFER_SIZE>
  static void writeInto(const int& pipe, const std::string& input);
#endif
#endif
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief readFrom reads from the specified pipe
   * @tparam BUFFER_SIZE is the buffer size for reading from the pipe
   * @param pipe is the pipe to read from
   * @return read value as a std::string
   */
  template <std::size_t BUFFER_SIZE>
  static std::string readFrom(const int& pipe);
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
   */
  void waitExitLoop();
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
  //! if waited -> actual exit status code value @see Process::wait()
  //! else -> empty exit status code value
  std::optional<int> exitCode_ = {};
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
  ret.stdinPipe_ = -1;
  //! reading from stdout of the same process is not supported yet ->
  //!     set stdout pipe to -1
  //! there will be an empty string value if stdout() is tried to be called
  ret.stdoutPipe_ = -1;
  //! reading stderr of the same process is not supported yet ->
  //!     set stderr pipe to -1
  //! there will be an empty string value if stderr() is tried to be called
  ret.stderrPipe_ = -1;
  //! return the process
  return ret;
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::optional<Process> Process::create(const Executable& executable) {
  const auto [argv, argvSize] = util::argvOf(executable);
  const auto [envp, envpSize] = util::envpOf(executable);
  auto argvRaw = std::make_unique<char*[]>(argvSize);
  for (auto i = 0; i < argvSize; i++) {
    argvRaw[i] = argv[i].get();
  }
  auto envpRaw = std::make_unique<char*[]>(envpSize);
  for (auto i = 0; i < envpSize; i++) {
    envpRaw[i] = envp[i].get();
  }
  int inFd[2];
  int outFd[2];
  int errFd[2];
  pipe(inFd);
  pipe(outFd);
  pipe(errFd);
  const auto pid = vfork();
  if (pid == 0) { //! forked process
    close(inFd[1]);
    close(outFd[0]);
    close(errFd[0]);
    dup2(inFd[0], STDIN_FILENO);
    dup2(outFd[1], STDOUT_FILENO);
    dup2(errFd[1], STDERR_FILENO);
    close(inFd[0]);
    close(outFd[1]);
    close(errFd[1]);
    const auto execRet = execve(argvRaw[0], argvRaw.get(), envpRaw.get());
    if (execRet != 0) {
      //! fail
      exit(errno);
    }
  }
  close(inFd[0]);
  close(outFd[1]);
  close(errFd[1]);
  if (pid < 0) { //! fork failed
    return {};
  }
  auto process = Process{};
  process.pid_ = pid;
  process.stdinPipe_ = inFd[1];
  process.stdoutPipe_ = outFd[0];
  process.stderrPipe_ = errFd[0];
  return std::optional<Process>(std::move(process));;
}
#endif
#endif

inline Process::~Process() {
#ifdef __unix__
#if __has_include(<unistd.h>)
  close(this->stdinPipe_);
  close(this->stdoutPipe_);
  close(this->stderrPipe_);
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
inline Process& Process::wait() {
  this->waitExitLoop();
  return *this;
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
#if __has_include(<unistd.h>)
inline void Process::stdin(const std::string& input) const {
  Process::writeInto<1024>(this->stdinPipe_, input);
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::string Process::stdout() const {
  return Process::readFrom<1024>(this->stdoutPipe_);
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::string Process::stderr() const {
  return Process::readFrom<1024>(this->stderrPipe_);
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
template <std::size_t BUFFER_SIZE>
void Process::writeInto(const int& pipe, const std::string& input) {
  char buffer[BUFFER_SIZE];
  for (auto i = 0; i <= input.size() / BUFFER_SIZE; i++) {
    const auto data = input.substr(i * BUFFER_SIZE, BUFFER_SIZE);
    std::size_t end;
    if (data.size() != BUFFER_SIZE) {
      end = data.size();
    } else {
      end = BUFFER_SIZE;
    }
    for (auto j = 0; j < end; j++) {
      buffer[j] = data[j];
    }
    write(pipe, buffer, end); //! no error handling
  }
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
template <std::size_t BUFFER_SIZE>
std::string Process::readFrom(const int& pipe) {
  auto oss = std::ostringstream{};
  ssize_t bytes;
  do {
    char buffer[BUFFER_SIZE];
    static_assert(BUFFER_SIZE > 1, "BUFFER_SIZE needs to have space for '\0'");
    bytes = read(pipe, buffer, BUFFER_SIZE - 1);
    if (bytes < 0) { //! read failed
      [[maybe_unused]] const auto& error = errno;
      //! no error handling
      return {};
    }
    buffer[bytes] = '\0';
    oss << std::move(buffer);
  } while (bytes == BUFFER_SIZE - 1);
  return oss.str();
}
#endif
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
inline void Process::waitExitLoop() {
  int status;
  while (true) {
    auto pid = waitpid(this->pid_, &status, WNOHANG);
    if (pid == 0) {
      continue;
    }
    if (pid == -1) {
      //! no error handling
      break;
    }
    if (WIFEXITED(status) == 0) {
      if (WIFSIGNALED(status) != 0) {
        [[maybe_unused]] const auto& sig = WTERMSIG(status);
        //! no error handling
      }
      if (WIFSTOPPED(status) != 0) {
        [[maybe_unused]] const auto& sig = WSTOPSIG(status);
        //! no error handling
      }
      break;
    }
    this->exitCode_ = WEXITSTATUS(status);
    break;
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
