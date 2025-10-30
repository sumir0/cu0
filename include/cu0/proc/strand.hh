#ifndef CU0_STRAND_HH_
#define CU0_STRAND_HH_

#if !__has_include(<pthread.h>)
#warning <pthread.h> is not found => \
cu0::Strand::priority_type will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::ResourceError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::GetPriorityError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::SetPriorityError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::Policy will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::GetPolicyError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::SetPolicyError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::RunError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::JoinError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::priority() will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::priority(const priority_type&) will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::policy() will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::policy(const Policy&) will not be supported
#endif

#include <functional>
#include <variant>
#include <utility>

#if __has_include(<pthread.h>)
  #include <cerrno>
  #include <pthread.h>
#else
  #include <thread>
#endif

namespace cu0 {

/*!
 * @brief The Strand struct provides a way to control a thread of a process
 */
struct Strand {
public:
#if __has_include(<pthread.h>)
  //! actual priority data representation
  using priority_type = std::remove_reference_t<
      decltype(std::declval<sched_param>().sched_priority)
  >;
#endif
#if __has_include(<pthread.h>)
  //! errors related to creation of a strand
  enum struct ResourceError {
    PTHREAD_NOMEM = ENOMEM, //! no memory
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to accesses of a priority value
  enum struct GetPriorityError {
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to changes of a priority value
  enum struct SetPriorityError {
    PTHREAD_INVAL = EINVAL, //! invalid priority was specified
  };
#endif
#if __has_include(<pthread.h>)
  //! available policies
  enum struct Policy {
    PTHREAD_OTHER = SCHED_OTHER,
    PTHREAD_IDLE = SCHED_IDLE,
    PTHREAD_BATCH = SCHED_BATCH,
    PTHREAD_FIFO = SCHED_FIFO,
    PTHREAD_RR = SCHED_RR,
    PTHREAD_DEADLINE = SCHED_DEADLINE,
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to accesses of a policy value
  enum struct GetPolicyError {
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to changes of a policy value
  enum struct SetPolicyError {
    PTHREAD_INVAL = EINVAL, //! invalid policy was specified
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to launches of a strand
  enum struct RunError {
    PTHREAD_AGAIN = EAGAIN, //! insufficient resources or limit
    PTHREAD_INVAL = EINVAL, //! invalid settings in attributes
    PTHREAD_PERM = EPERM, //! no permission to set parameters in attributes
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to joins of a strand
  enum struct JoinError {
    PTHREAD_DEADLK = EDEADLK, //! deadlock detected
    PTHREAD_INVAL = EINVAL, //! thread is already being waited to join
    PTHREAD_SRCH = ESRCH, //! thread was not found
  };
#endif
  /*
   * @brief creates a strand instance
   * @param task is the task to be executed after launch @see Strand::run()
   * @return
   *     if no error was reported => strand instance
   *     else => error code
   */
  [[nodiscard]] static
#if __has_include(<pthread.h>)
      std::variant<Strand, ResourceError>
#else
      std::variant<Strand>
#endif
      create(std::function<void()> task);
#if __has_include(<pthread.h>)
  /*!
   * @brief gets a priority with which this strand will be launched
   * @note don't call this function after call to Strand::run() unless
   *     the consequnces of that calls are well understood
   * @return
   *     if no error was reported => priority value
   *     else => error code
   */
  [[nodiscard]]
  constexpr std::variant<priority_type, GetPriorityError> priority() const;
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief sets a priority with which this strand will be launched
   * @note don't call this function after call to Strand::run() unless
   *     the consequnces of that calls are well understood
   * @param value is the priority to be set @note policy-specific
   *     use sched_get_priority_min(static_cast<int>(Policy)) and
   *     sched_get_priority_max(static_cast<int>(Policy)) to get minimal and
   *     maximal values for the specified policy
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  constexpr std::variant<std::monostate, SetPriorityError> priority(
      const priority_type& value
  );
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief gets a policy type with which this strand will be launched
   * @note don't call this function after call to Strand::run() unless
   *     the consequnces of that calls are well understood
   * @return
   *     if no error was reported => policy type
   *     else => error code
   */
  [[nodiscard]]
  constexpr std::variant<Policy, GetPolicyError> policy() const;
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief sets a policy type with which this strand will be launched
   * @note don't call this function after call to Strand::run() unless
   *     the consequnces of that calls are well understood
   * @param value is the policy type to be set
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  constexpr std::variant<std::monostate, SetPolicyError> policy(
      const Policy& value
  );
#endif
  /*!
   * @brief runs a task specified by this->task_, i.e. launches this strand
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
#if __has_include(<pthread.h>)
  std::variant<std::monostate, RunError, ResourceError>
#else
  std::variant<std::monostate>
#endif
      run();
  /*!
   * @brief joins to this strand, i.e. waits for the strand to exit
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
#if __has_include(<pthread.h>)
  constexpr std::variant<std::monostate, JoinError>
#else
  std::variant<std::monostate>
#endif
      join();
protected:
  /*!
   * @brief default ctor
   */
#if __has_include(<pthread.h>)
  constexpr
#else
  inline
#endif
      Strand() = default;
#if __has_include(<pthread.h>)
  /*!
   * @brief runs task specified by reinterpret_cast<Strand const *>(args)->task_
   * @note used as a helper to launch a strand
   * @param args is the pointer to the instance of this type
   * @return nullptr
   */
  static void* task(void* args);
#endif
  //! task to be executed after launch
  std::function<void()> task_{};
#if __has_include(<pthread.h>)
  //! parameters of a strand launch
  //! @see Strand::priority(), Strand::policy()
  pthread_attr_t attr_{};
#endif
  //! thread
#if __has_include(<pthread.h>)
  pthread_t thread_{};
#else
  std::thread thread_{};
#endif
};

} /// namespace cu0

namespace cu0 {

inline
#if __has_include(<pthread.h>)
std::variant<Strand, typename Strand::ResourceError>
#else
std::variant<Strand>
#endif
Strand::create(
    std::function<void()> task
) {
  auto ret = Strand{};
  ret.task_ = std::move(task);
#if __has_include(<pthread.h>)
  const auto attrInitStatus = pthread_attr_init(&ret.attr_);
  if (attrInitStatus != 0) {
    return static_cast<ResourceError>(attrInitStatus);
  }
  pthread_attr_setinheritsched(&ret.attr_, PTHREAD_EXPLICIT_SCHED);
#endif
  return ret;
}

#if __has_include(<pthread.h>)
constexpr std::variant<
    typename Strand::priority_type,
    typename Strand::GetPriorityError
> Strand::priority() const {
  sched_param schedParam;
  const auto res = pthread_attr_getschedparam(
      &this->attr_,
      &schedParam
  );
  if (res != 0) {
    return static_cast<GetPriorityError>(res);
  }
  return schedParam.sched_priority;
}
#endif

#if __has_include(<pthread.h>)
constexpr std::variant<std::monostate, typename Strand::SetPriorityError>
Strand::priority(const priority_type& value) {
  const auto schedParam = sched_param{ .sched_priority = value, };
  const auto res = pthread_attr_setschedparam(
      &this->attr_,
      &schedParam
  );
  if (res != 0) {
    return static_cast<SetPriorityError>(res);
  }
  return std::monostate{};
}
#endif

#if __has_include(<pthread.h>)
constexpr std::variant<
    typename Strand::Policy,
    typename Strand::GetPolicyError
> Strand::policy() const {
  int nativePolicy;
  const auto res = pthread_attr_getschedpolicy(&this->attr_, &nativePolicy);
  if (res != 0) {
    return static_cast<GetPolicyError>(res);
  }
  return static_cast<Policy>(nativePolicy);
}
#endif

#if __has_include(<pthread.h>)
constexpr std::variant<std::monostate, typename Strand::SetPolicyError>
Strand::policy(const Policy& policy) {
  const auto res = pthread_attr_setschedpolicy(
      &this->attr_,
      static_cast<int>(policy)
  );
  if (res != 0) {
    return static_cast<SetPolicyError>(res);
  }
  return std::monostate{};
}
#endif

inline
#if __has_include(<pthread.h>)
std::variant<
    std::monostate,
    typename Strand::RunError,
    typename Strand::ResourceError
>
#else
std::variant<std::monostate>
#endif
Strand::run() {
#if __has_include(<pthread.h>)
  const auto errorCode = pthread_create(
      &this->thread_,
      &this->attr_,
      &Strand::task,
      this
  );
  if (errorCode != 0) {
    return static_cast<RunError>(errorCode);
  }
  const auto attrDestroyResult = pthread_attr_destroy(&this->attr_);
  if (attrDestroyResult != 0) {
    return static_cast<ResourceError>(attrDestroyResult);
  }
#else
  this->thread_ = std::thread(this->task_);
#endif
  return std::monostate{};
}

#if __has_include(<pthread.h>)
constexpr std::variant<std::monostate, typename Strand::JoinError>
#else
std::variant<std::monostate>
#endif
Strand::join() {
#if __has_include(<pthread.h>)
  void* res;
  const auto errorCode = pthread_join(this->thread_, &res);
  if (errorCode != 0) {
    return static_cast<JoinError>(errorCode);
  }
#else
  this->thread_.join();
#endif
  return std::monostate{};
}

#if __has_include(<pthread.h>)
inline void* Strand::task(void* args) {
  reinterpret_cast<Strand const *>(args)->task_();
  return nullptr;
}
#endif

} /// namespace cu0

#endif /// CU0_STRAND_HH_
