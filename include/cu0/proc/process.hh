#ifndef CU0_PROCESS_HH_
#define CU0_PROCESS_HH_

/*!
 * @brief checks software compatibility during compile-time
 */
#ifdef __unix__
#if !__has_include(<unistd.h>)
#warning <unistd.h> not found => cu0::Process::current() will not be supported
#else
#include <unistd.h>
#endif
#else
#warning __unix__ is not defined => \
    cu0::Process::current() will not be supported
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

constexpr const unsigned& Process::pid() const {
  return this->pid_;
}

} /// namespace cu0

#endif /// CU0_PROCESS_HH_
