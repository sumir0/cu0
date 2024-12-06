#ifndef CU0_PROCESS_HH_
#define CU0_PROCESS_HH_

#include <optional>
#include <sstream>

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
    cu0::Process::stdout() will not be supported
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
    cu0::Process::stdout() will not be supported
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
  virtual constexpr ~Process() = default;
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
   * @brief stdout returns a copy of stdout
   * @return stdout copy as a std::istringstream
   */
  std::istringstream stdout() const;
#endif
#endif
protected:
#ifdef __unix__
#if __has_include(<unistd.h>)
  /*!
   * @brief stdout returns a copy of stdout
   * @tparam BUFFER_SIZE is the buffer size for reading from pipe
   * @return stdout copy as a std::istringstream
   */
  template <std::size_t BUFFER_SIZE>
  static std::istringstream readFrom(const int& pipe);
#endif
#endif
  /*!
   * @brief constructs an instance with default values
   */
  constexpr Process() = default;
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
  //! stdout file descriptor
  int stdoutPipe_ = 0;
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
  //! set process identifier of the process
  ret.pid_ = getpid();
  //! return the process
  return ret;
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
inline std::optional<Process> Process::create(const Executable& executable) {
  auto ret = Process{};
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
    dup2(inFd[0], 0);
    dup2(outFd[1], 1);
    dup2(errFd[1], 2);
    close(inFd[0]);
    close(outFd[1]);
    close(errFd[1]);
    const auto execRet = execve(argvRaw[0], argvRaw.get(), envpRaw.get());
    if (execRet != 0) {
      //! fail
      exit(errno);
    }
  }
  close(inFd[0]); //! @dev @note possible fail
  close(outFd[1]);
  close(errFd[1]);
  if (pid < 0) { //! fork failed
    return {};
  }
  ret.pid_ = pid;
  ret.stdoutPipe_ = outFd[0];
  return std::optional<Process>(std::move(ret));
}
#endif
#endif

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
std::istringstream Process::stdout() const {
  return Process::readFrom<1024>(this->stdoutPipe_);
}
#endif
#endif

#ifdef __unix__
#if __has_include(<unistd.h>)
template <std::size_t BUFFER_SIZE>
std::istringstream Process::readFrom(const int& pipe) {
  auto oss = std::ostringstream{};
  ssize_t bytes;
  do {
    char buffer[BUFFER_SIZE];
    static_assert(BUFFER_SIZE > 1, "BUFFER_SIZE needs to have space for '\0'");
    bytes = read(pipe, buffer, BUFFER_SIZE - 1);
    if (bytes < 0) { //! read failed
      return {};
    }
    buffer[bytes] = '\0';
    oss << std::move(buffer);
  } while (bytes == BUFFER_SIZE - 1);
  return std::istringstream{oss.str()};
}
#endif
#endif

#ifdef __unix__
#if __has_include(<sys/types.h>) && __has_include(<sys/wait.h>)
inline void Process::waitExitLoop() {
  int status;
  while (true) {
    auto pid = waitpid(this->pid_, &status, WNOHANG);
    if (pid != 0) {
      if (pid != -1 && WIFEXITED(status) != 0) {
        this->exitCode_ = WEXITSTATUS(status);
      }
      break;
    }
  }
}
#endif
#endif

} /// namespace cu0

#endif /// CU0_PROCESS_HH_
