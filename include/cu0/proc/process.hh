#ifndef CU0_PROCESS_HH_
#define CU0_PROCESS_HH_

#include <cu0/proc/executable.hh>

/*!
 * @brief checks software compatibility during compile-time
 */
#ifdef __unix__
#if !__has_include(<unistd.h>)
#warning <unistd.h> not found => cu0::Process::current() will not be supported
#warning <unistd.h> not found => cu0::Process::create() will not be supported
#else
#include <unistd.h>
#include <optional> //! for Process::create()
#endif
#else
#warning __unix__ is not defined => \
    cu0::Process::current() will not be supported
#warning __unix__ is not defined => \
    cu0::Process::create() will not be supported
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
   * @brief accesses process identifier value
   * @return process identifier as a const reference
   */
  constexpr const unsigned& pid() const;
protected:
  /*!
   * @brief constructs an instance with default values
   */
  constexpr Process() = default;
  //! process identifier
  unsigned pid_ = 0;
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
  const auto pid = vfork();
  if (pid == 0) {
    const auto execRet = execve(argvRaw[0], argvRaw.get(), envpRaw.get());
    if (execRet != 0) {
      //! fail
      exit(errno);
    }
  }
  if (pid < 0) {
    return {};
  }
  ret.pid_ = pid;
  return ret;
}
#endif
#endif

constexpr const unsigned& Process::pid() const {
  return this->pid_;
}

} /// namespace cu0

#endif /// CU0_PROCESS_HH_
