#ifndef CU0_THREAD_SCHEDULING_HH_
#define CU0_THREAD_SCHEDULING_HH_

#include <tuple>
#include <utility>

#ifdef __unix__
#if __has_include(<pthread.h>)
  #include <cerrno>
  #include <pthread.h>
#endif
#endif

namespace cu0 {

template <class Type>
concept HasNativeHandle = requires {
  typename Type::native_handle_type;
};

/*!
 * @brief The ThreadScheduling struct provides a way to control scheduling of a
 *     thread
 */
template <class Thread>
requires HasNativeHandle<Thread>
struct ThreadScheduling {
public:
  enum struct Policy {
#ifdef __unix__
    UNIX_OTHER,
    UNIX_IDLE,
    UNIX_BATCH,
    UNIX_FIFO, //! first in first out
    UNIX_ROUND_ROBIN, //! FIFO with a maximum running time quantum
    UNIX_DEADLINE,
#endif
    UNSUPPORTED, //! unsupported
  };
  enum struct GetPriorityStatus {
    DONE, //! a priority was got and no errors were reported
    NOT_SUPPORTED, //! priority is not supported on this platform
    INVALID_ID, //! there is no thread with the native handle @see nativeHandle_
    UNKNOWN, //! no information about the status is known, unexpected behaviour
  };
  enum struct SetPriorityStatus {
    DONE, //! a priority was set and no errors were reported
    NOT_SUPPORTED, //! @see GetPriorityStatus::NOT_SUPPORTED
    INVALID_ID, //! @see GetPriorityStatus::INVALID_ID
    INVALID_POLICY, //! the specified policy is not supported
    INVALID_VALUE, //! the specified value or policy is not valid
    INVALID_PERMISSIONS, //! user does not have appropriate privileges
    UNKNOWN, //! @see GetPriorityStatus::UNKNOWN
  };
  /*!
   * @brief default ctor
   */
  constexpr ThreadScheduling() = default;
  /*!
   * @brief ctor using a native handle
   * @param nativeHandle is the native handle of a thread
   */
  constexpr ThreadScheduling(typename Thread::native_handle_type nativeHandle);
  /*!
   * @brief gets native handle
   * @return native handle
   */
  [[nodiscard]]
  constexpr const typename Thread::native_handle_type& nativeHandle() const;
  /*!
   * @brief gets a priority of the thread @see nativeHandle_
   * @return
   *    if a priority value is accessible ->
   *        { priority value, GetPriorityStatus::DONE, }
   *    else -> { 0, error, }
   */
  [[nodiscard]]
  constexpr std::tuple<int, GetPriorityStatus> priority() const;
  /*!
   * @brief sets a priority of the thread @see nativeHandle_
   * @param policy is the policy of the thread to be set @see Policy
   * @param value is the priority to be set @note policy-specific,
   *     some policies do not use the priority value
   * @return
   *    if a priority was set -> SetPriorityStatus::DONE
   *    else -> error value
   */
  constexpr SetPriorityStatus priority(const Policy& policy, const int& value);
protected:
  //! native handle of the thread controlled by this instance
  //! @see nativeHandle()
  typename Thread::native_handle_type nativeHandle_{};
};

} /// namespace cu0

namespace cu0 {

template <class Thread>
requires HasNativeHandle<Thread>
constexpr ThreadScheduling<Thread>::ThreadScheduling(
    typename Thread::native_handle_type nativeHandle
) : nativeHandle_{std::move(nativeHandle)} {}

template <class Thread>
requires HasNativeHandle<Thread>
constexpr const typename Thread::native_handle_type&
ThreadScheduling<Thread>::nativeHandle() const {
  return this->nativeHandle_;
}

template <class Thread>
requires HasNativeHandle<Thread>
constexpr std::tuple<int, typename ThreadScheduling<Thread>::GetPriorityStatus>
ThreadScheduling<Thread>::priority() const {
#ifdef __unix__
#if __has_include(<pthread.h>)
  sched_param value;
  int nativePolicy;
  const auto res =
      pthread_getschedparam(this->nativeHandle(), &nativePolicy, &value);
  switch (res) {
  case 0: //! no error
    return { value.sched_priority, ThreadScheduling::GetPriorityStatus::DONE, };
  case ESRCH:
    return { 0, ThreadScheduling::GetPriorityStatus::INVALID_ID, };
  default:
    return { 0, ThreadScheduling::GetPriorityStatus::UNKNOWN, };
  }
#endif
#endif
  return { 0, ThreadScheduling::GetPriorityStatus::NOT_SUPPORTED, };
}

template <class Thread>
requires HasNativeHandle<Thread>
constexpr typename ThreadScheduling<Thread>::SetPriorityStatus
ThreadScheduling<Thread>::priority(
    const typename ThreadScheduling::Policy& policy,
    const int& value
) {
#ifdef __unix__
#if __has_include(<pthread.h>)
  const auto schedParam = sched_param{ .sched_priority = value, };
  int nativePolicy;
  switch (policy) {
  case ThreadScheduling::Policy::UNIX_OTHER:
    nativePolicy = SCHED_OTHER;
    break;
  case ThreadScheduling::Policy::UNIX_IDLE:
    nativePolicy = SCHED_IDLE;
    break;
  case ThreadScheduling::Policy::UNIX_BATCH:
    nativePolicy = SCHED_BATCH;
    break;
  case ThreadScheduling::Policy::UNIX_FIFO:
    nativePolicy = SCHED_FIFO;
    break;
  case ThreadScheduling::Policy::UNIX_ROUND_ROBIN:
    nativePolicy = SCHED_RR;
    break;
  case ThreadScheduling::Policy::UNIX_DEADLINE:
    nativePolicy = SCHED_DEADLINE;
    break;
  default:
    return ThreadScheduling::SetPriorityStatus::INVALID_POLICY;
  }
  const auto res =
      pthread_setschedparam(this->nativeHandle(), nativePolicy, &schedParam);
  switch (res) {
  case 0: //! no error
    return ThreadScheduling::SetPriorityStatus::DONE;
  case ESRCH:
    return ThreadScheduling::SetPriorityStatus::INVALID_ID;
  case EINVAL:
  case ENOTSUP:
    return ThreadScheduling::SetPriorityStatus::INVALID_VALUE;
  case EPERM:
    return ThreadScheduling::SetPriorityStatus::INVALID_PERMISSIONS;
  default:
    return ThreadScheduling::SetPriorityStatus::UNKNOWN;
  }
#endif
#endif
  return ThreadScheduling::SetPriorityStatus::NOT_SUPPORTED;
}

} /// namespace cu0

#endif /// CU0_THREAD_SCHEDULING_HH_
