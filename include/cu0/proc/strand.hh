#ifndef CU0_STRAND_HH__
#define CU0_STRAND_HH__

#if !__has_include(<pthread.h>)
#warning <pthread.h> is not found => \
cu0::Strand::Policy will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::priority_type will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::Scheduling will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::Stage will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::ResourceError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::GetPriorityError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::SetPriorityError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::GetPolicyError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::SetPolicyError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::InitError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::RunError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::JoinError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::priority() will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::priority(const priority_type&) will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::scheduling() will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::scheduling(const Scheduling&) will not be supported
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
  //! available policies
  enum struct Policy {
    PTHREAD_OTHER = SCHED_OTHER, /// @see SCHED_OTHER
    PTHREAD_FIFO = SCHED_FIFO, /// @see SCHED_FIFO
    PTHREAD_RR = SCHED_RR, /// @see SCHED_RR
  };
#endif
#if __has_include(<pthread.h>)
  //! underlying priority type
  using priority_type = std::remove_reference_t<
      decltype(std::declval<sched_param>().sched_priority)
  >;
#endif
#if __has_include(<pthread.h>)
  //! scheduling data representation
  struct Scheduling {
    Policy policy = Policy::PTHREAD_OTHER;
    /*!
     * @note policy-specific
     *     when it is modified use
     *     sched_get_priority_min(static_cast<int>(Policy)) and
     *     sched_get_priority_max(static_cast<int>(Policy)) to get minimal and
     *     maximal values for the specified policy
     */
    priority_type priority{};
  };
#endif
#if __has_include(<pthread.h>)
  //! stage of a strand
  enum struct Stage {
    NOT_LAUNCHED, //! before Strand::run()
    LAUNCHED, //! after Strand::run() but before Strand::join()
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to resource management of a strand
  enum struct ResourceError {
    PTHREAD_NOMEM = ENOMEM, //! no memory
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to retrieval of a priority value
  enum struct GetPriorityError {
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to modification of a priority value
  enum struct SetPriorityError {
    PTHREAD_NOTSUP = ENOTSUP, //! unsupported priority or policy was specified
    PTHREAD_INVAL = EINVAL, //! invalid priority or policy was specified
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to retrieval of a policy value
  enum struct GetPolicyError {
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to modification of a policy value
  enum struct SetPolicyError {
    PTHREAD_NOTSUP = ENOTSUP, //! unsupported policy was specified
    PTHREAD_INVAL = EINVAL, //! invalid policy was specified
    PTHREAD_PERM = EPERM, //! current privileges are not enough
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to initializtion of a strand and its parameters
  enum struct InitError {
    //! operation unsupported on the platform was requested
    //! changing scheduling and/or priority will not make effect
    PTHREAD_NOTSUP = ENOTSUP,
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to launching of a strand
  enum struct RunError {
    PTHREAD_AGAIN = EAGAIN, //! insufficient resources or limit
    PTHREAD_PERM = EPERM, //! no permission to apply strand parameters
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to joining to a strand
  enum struct JoinError {
    PTHREAD_DEADLK = EDEADLK, //! deadlock was detected
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
      std::variant<Strand, ResourceError, InitError>
#else
      std::variant<Strand>
#endif
      create(std::function<void()> task);
#if __has_include(<pthread.h>)
  /*!
   * @brief gets a priority of this strand
   * @note deleted | actual implementations are provided through specializations
   * @return
   *     if no error was reported => priority value
   *     else => error code
   */
  template <Stage stage>
  [[nodiscard]]
  constexpr std::variant<
      priority_type,
      GetPriorityError
  > priority() const = delete;
#ifndef CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
  /*!
   * @brief gets a priority with which this strand will be launched
   * @return
   *     if no error was reported => priority value
   *     else => error code
   */
  template <>
  [[nodiscard]]
  constexpr std::variant<
      priority_type,
      GetPriorityError
  > priority<Stage::NOT_LAUNCHED>() const;
  /*!
   * @brief gets a priority with which this strand is running
   * @return
   *     if no error was reported => priority value
   *     else => error code
   */
  template <>
  [[nodiscard]]
  constexpr std::variant<
      priority_type,
      GetPriorityError
  > priority<Stage::LAUNCHED>() const;
#endif
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief sets a priority of this strand
   * @note deleted | actual implementations are provided through specializations
   * @param priority is the priority to be set
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <Stage stage>
  constexpr std::variant<std::monostate, SetPriorityError> priority(
      const priority_type& priority
  ) = delete;
#ifndef CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
  /*!
   * @brief sets a priority with which this strand will be launched
   * @param priority is the priority to be set
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <>
  constexpr std::variant<std::monostate, SetPriorityError> priority<
      Stage::NOT_LAUNCHED
  >(const priority_type& priority);
  /*!
   * @brief sets a priority with which this strand will continue to run
   * @param priority is the priority to be set
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <>
  constexpr std::variant<std::monostate, SetPriorityError> priority<
      Stage::LAUNCHED
  >(const priority_type& priority);
#endif
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief gets scheduling parameters of this strand
   * @note deleted | actual implementations are provided through specializations
   * @return
   *     if no error was reported => scheduling parameters
   *     else => error code
   */
  template <Stage stage>
  [[nodiscard]]
  constexpr std::variant<
      Scheduling,
      GetPolicyError,
      GetPriorityError
  > scheduling() const = delete;
#ifndef CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
  /*!
   * @brief gets scheduling parameters with which this strand will be launched
   * @return
   *     if no error was reported => scheduling parameters
   *     else => error code
   */
  template <>
  [[nodiscard]]
  constexpr std::variant<
      Scheduling,
      GetPolicyError,
      GetPriorityError
  > scheduling<Stage::NOT_LAUNCHED>() const;
  /*!
   * @brief gets scheduling parameters with which this strand is running
   * @note GetPriorityError is not used by this implementation
   * @return
   *     if no error was reported => scheduling parameters
   *     else => error code
   */
  template <>
  [[nodiscard]]
  constexpr std::variant<
      Scheduling,
      GetPolicyError,
      GetPriorityError
  > scheduling<Stage::LAUNCHED>() const;
#endif
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief sets scheduling parameters of this strand
   * @note deleted | actual implementations are provided through specializations
   * @param scheduling is the scheduling parameters to be set
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <Stage stage>
  constexpr std::variant<
      std::monostate,
      SetPolicyError,
      SetPriorityError
  > scheduling(const Scheduling& scheduling) = delete;
#ifndef CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
  /*!
   * @brief sets scheduling parameters with which this strand will be launched
   * @param scheduling is the scheduling parameters to be set
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <>
  constexpr std::variant<
      std::monostate,
      SetPolicyError,
      SetPriorityError
  > scheduling<Stage::NOT_LAUNCHED>(const Scheduling& scheduling);
  /*!
   * @brief sets scheduling parameters with which this strand will continue to
   *     run
   * @note SetPriorityError is not used by this implementation
   * @param scheduling is the scheduling parameters to be set
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <>
  constexpr std::variant<
      std::monostate,
      SetPolicyError,
      SetPriorityError
  > scheduling<Stage::LAUNCHED>(const Scheduling& scheduling);
#endif
#endif
  /*!
   * @brief runs a task specified by this->task_ in a new thread,
   *     i.e. launches this strand
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
   * @brief calls reinterpret_cast<Strand const *>(args)->task_ task
   * @note used as a helper to launch a strand
   * @param args is the pointer to the instance of this type
   * @return nullptr
   */
  static void* task(void* args);
#endif
  //! task to be executed after launch
  std::function<void()> task_{};
#if __has_include(<pthread.h>)
  //! internal representation of parameters with which a strand is launched
  //! @see Strand::Scheduling
  pthread_attr_t attr_{};
#endif
  //! thread implementation
#if __has_include(<pthread.h>)
  pthread_t
#else
  std::thread
#endif
      thread_{};
};

} /// namespace cu0

namespace cu0 {

inline
#if __has_include(<pthread.h>)
std::variant<Strand, typename Strand::ResourceError, typename Strand::InitError>
#else
std::variant<Strand>
#endif
Strand::create(
    std::function<void()> task
) {
  auto ret = Strand{};
  ret.task_ = std::move(task);
#if __has_include(<pthread.h>)
  const auto attrInitResult = pthread_attr_init(&ret.attr_);
  if (attrInitResult != 0) {
    return static_cast<ResourceError>(attrInitResult);
  }
  const auto setInheritSchedResult =
      pthread_attr_setinheritsched(&ret.attr_, PTHREAD_EXPLICIT_SCHED);
  if (setInheritSchedResult != 0) {
    return static_cast<InitError>(setInheritSchedResult);
  }
#endif
  return ret;
}

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    typename Strand::priority_type,
    typename Strand::GetPriorityError
> Strand::priority<Strand::Stage::NOT_LAUNCHED>() const {
  auto schedParam = sched_param{};
  const auto res = pthread_attr_getschedparam(&this->attr_, &schedParam);
  if (res != 0) {
    return static_cast<GetPriorityError>(res);
  }
  return schedParam.sched_priority;
}

template <>
constexpr std::variant<
    typename Strand::priority_type,
    typename Strand::GetPriorityError
> Strand::priority<Strand::Stage::LAUNCHED>() const {
  auto schedParam = sched_param{};
  auto nativePolicy = int{};
  const auto res =
      pthread_getschedparam(this->thread_, &nativePolicy, &schedParam);
  if (res != 0) {
    return static_cast<GetPriorityError>(res);
  }
  return schedParam.sched_priority;
}
#endif

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    std::monostate,
    typename Strand::SetPriorityError
> Strand::priority<Strand::Stage::NOT_LAUNCHED>(const priority_type& priority) {
  const auto schedParam = sched_param{ .sched_priority = priority, };
  const auto res = pthread_attr_setschedparam(
      &this->attr_,
      &schedParam
  );
  if (res != 0) {
    return static_cast<SetPriorityError>(res);
  }
  return std::monostate{};
}

template <>
constexpr std::variant<
    std::monostate,
    typename Strand::SetPriorityError
> Strand::priority<Strand::Stage::LAUNCHED>(const priority_type& priority) {
  const auto res = pthread_setschedprio(this->thread_, priority);
  if (res != 0) {
    return static_cast<SetPriorityError>(res);
  }
  return std::monostate{};
}
#endif

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    typename Strand::Scheduling,
    typename Strand::GetPolicyError,
    typename Strand::GetPriorityError
> Strand::scheduling<Strand::Stage::NOT_LAUNCHED>() const {
  auto nativePolicy = int{};
  auto schedParam = sched_param{};
  const auto policyGetResult =
      pthread_attr_getschedpolicy(&this->attr_, &nativePolicy);
  if (policyGetResult != 0) {
    return static_cast<GetPolicyError>(policyGetResult);
  }
  const auto priorityGetResult =
      pthread_attr_getschedparam(&this->attr_, &schedParam);
  if (priorityGetResult != 0) {
    return static_cast<GetPriorityError>(priorityGetResult);
  }
  return Scheduling{
    .policy = static_cast<Policy>(nativePolicy),
    .priority = schedParam.sched_priority,
  };
}

template <>
constexpr std::variant<
    typename Strand::Scheduling,
    typename Strand::GetPolicyError,
    typename Strand::GetPriorityError
> Strand::scheduling<Strand::Stage::LAUNCHED>() const {
  auto nativePolicy = int{};
  auto schedParam = sched_param{};
  const auto res =
      pthread_getschedparam(this->thread_, &nativePolicy, &schedParam);
  if (res != 0) {
    return static_cast<GetPolicyError>(res);
  }
  return Scheduling{
    .policy = static_cast<Policy>(nativePolicy),
    .priority = schedParam.sched_priority,
  };
}
#endif

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    std::monostate,
    typename Strand::SetPolicyError,
    typename Strand::SetPriorityError
> Strand::scheduling<Strand::Stage::NOT_LAUNCHED>(
    const Scheduling& scheduling
) {
  const auto schedParam = sched_param{ .sched_priority = scheduling.priority, };
  const auto policySetResult = pthread_attr_setschedpolicy(
      &this->attr_,
      static_cast<int>(scheduling.policy)
  );
  if (policySetResult != 0) {
    return static_cast<SetPolicyError>(policySetResult);
  }
  const auto prioritySetResult = pthread_attr_setschedparam(
      &this->attr_,
      &schedParam
  );
  if (prioritySetResult != 0) {
    return static_cast<SetPriorityError>(prioritySetResult);
  }
  return std::monostate{};
}

template <>
constexpr std::variant<
    std::monostate,
    typename Strand::SetPolicyError,
    typename Strand::SetPriorityError
> Strand::scheduling<Strand::Stage::LAUNCHED>(const Scheduling& scheduling) {
  const auto schedParam = sched_param{ .sched_priority = scheduling.priority, };
  const auto res = pthread_setschedparam(
      this->thread_,
      static_cast<int>(scheduling.policy),
      &schedParam
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

#endif /// CU0_STRAND_HH__
