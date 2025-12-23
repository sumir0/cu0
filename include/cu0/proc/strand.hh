#ifndef CU0_STRAND_HH__
#define CU0_STRAND_HH__

#if !__has_include(<pthread.h>)
#warning <pthread.h> is not found => \
cu0::Strand::Policy will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::PriorityType will not be supported
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
cu0::Strand::GetDetachedError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::SetDetachedError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::InitError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::RunError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::JoinError will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::priority() will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::priority(const PriorityType&) will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::scheduling() will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::scheduling(const Scheduling&) will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::detached() will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::detached(bool) will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::stack_size() will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::allocate_stack(std::size_t) will not be supported
#warning <pthread.h> is not found => \
cu0::Strand::deallocate_stack() will not be supported
#endif
#if !__has_include(<stdlib.h>)
#warning <stdlib.h> is not found => \
cu0::Strand::allocate_stack(std::size_t) will not be supported
#warning <stdlib.h> is not found => \
cu0::Strand::deallocate_stack() will not be supported
#endif
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
cu0::Strand::allocate_stack(std::size_t) will not be supported
#warning <unistd.h> is not found => \
cu0::Strand::deallocate_stack() will not be supported
#endif

#include <functional>
#include <utility>
#include <variant>

#if __has_include(<pthread.h>)
  #include <cerrno>
  #include <pthread.h>
#else
  #include <thread>
#endif

#if __has_include(<stdlib.h>)
  #include <stdlib.h>
#endif

#if __has_include(<unistd.h>)
  #include <unistd.h>
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
  using PriorityType = std::remove_reference_t<
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
    PriorityType priority{};
  };
#endif
#if __has_include(<pthread.h>)
  //! stage of a strand
  enum struct Stage {
    NOT_LAUNCHED, //! before `Strand::run()`
    LAUNCHED, //! after `Strand::run()` but before execution terminates
    TERMINATED, //! after execution terminates
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
  //! errors related to retrieval of a status of the detached state
  enum struct GetDetachedError {
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to modification of a status of the detached state
  enum struct SetDetachedError {
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to retrieval of a stack size
  enum struct GetStackSizeError {
  };
#endif
#if __has_include(<pthread.h>)
  //! errors related to modification of a stack size
  enum struct SetStackSizeError {
    PTHREAD_INVAL = EINVAL, //! bad aligned or small or big size was specified
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
#if __has_include(<pthread.h>)
  //! errors related to detaching of a strand
  enum struct DetachError {
    PTHREAD_INVAL = EINVAL, //! strand is not detachable
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
      PriorityType,
      GetPriorityError
  > priority() const = delete;
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
  /*!
   * @brief gets a priority with which this strand will be launched
   * @return
   *     if no error was reported => priority value
   *     else => error code
   */
  template <>
  [[nodiscard]]
  constexpr std::variant<
      PriorityType,
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
      PriorityType,
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
      const PriorityType& priority
  ) = delete;
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
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
  >(const PriorityType& priority);
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
  >(const PriorityType& priority);
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
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
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
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
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
#if __has_include(<pthread.h>)
  /*!
   * @brief gets a status of the detached state of this strand
   * @note deleted | actual implementations are provided through specializations
   * @return
   *     if no error was reported => status of the detached state
   *     else => error code
   */
  template <Stage stage>
  [[nodiscard]]
  constexpr std::variant<
      bool,
      GetDetachedError
  > detached() const = delete;
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
  /*!
   * @brief gets a status of the detached state with which this strand will be
   *     launched
   * @return
   *     if no error was reported => status of the detached state
   *     else => error code
   */
  template <>
  [[nodiscard]]
  constexpr std::variant<
      bool,
      GetDetachedError
  > detached<Stage::NOT_LAUNCHED>() const;
#endif
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief sets a status of the detached state of this strand
   * @note deleted | actual implementations are provided through specializations
   * @param detached is the flag specifying if this strand needs to be detached
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <Stage stage>
  constexpr std::variant<
      std::monostate,
      SetDetachedError
  > detached(const bool detached) = delete;
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
  /*!
   * @brief sets a status of the detached state with which this strand will be
   *     launched
   * @param detached is the flag specifying if this strand needs to be detached
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <>
  constexpr std::variant<
      std::monostate,
      SetDetachedError
  > detached<Stage::NOT_LAUNCHED>(const bool detached);
#endif
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief gets stack size to be allocated for this strand
   * @note deleted | actual implementations are provided through specializations
   * @return
   *     if no error was reported => stack size in bytes
   *     else => error code
   */
  template <Stage stage>
  [[nodiscard]]
  constexpr std::variant<
      std::size_t,
      GetStackSizeError
  > stack_size() const = delete;
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
  /*!
   * @brief gets stack size to be allocated for this strand before launch
   * @return
   *     if no error was reported => stack size in bytes
   *     else => error code
   */
  template <>
  [[nodiscard]]
  constexpr std::variant<
      std::size_t,
      GetStackSizeError
  > stack_size<Stage::NOT_LAUNCHED>() const;
#endif
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief allocates stack for this strand
   * @note deleted | actual implementations are provided through specializations
   * @param stack_size is the size of a stack to be allocated in bytes
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <Stage stage>
  constexpr std::variant<
      std::monostate,
      SetStackSizeError,
      ResourceError
  > allocate_stack(const std::size_t stack_size) = delete;
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
#if __has_include(<stdlib.h>) && __has_include(<unistd.h>)
  /*!
   * @brief allocates stack for this strand before launch
   * @note allocated stack should be deallocated @see `deallocate_stack`
   * @param stack_size is the size of a stack to be allocated in bytes
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
  template <>
  std::variant<
      std::monostate,
      SetStackSizeError,
      ResourceError
  > allocate_stack<Stage::NOT_LAUNCHED>(const std::size_t stack_size);
#endif
#endif
#endif
#if __has_include(<pthread.h>)
  /*!
   * @brief deallocates stack from this strand
   * @note deleted | actual implementations are provided through specializations
   * @return
   *     if no error was reported => std::monostate
   */
  template <Stage stage>
  constexpr std::variant<std::monostate> deallocate_stack() = delete;
#if !CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT
#if __has_include(<stdlib.h>) && __has_include(<unistd.h>)
  /*!
   * @brief deallocates stack from this strand
   * @note use after `join` if `allocateStack` was called
   * @return
   *     if no error was reported => std::monostate
   */
  template <>
  std::variant<std::monostate> deallocate_stack<Stage::TERMINATED>();
#endif
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
  /*!
   * @brief detaches this strand
   * @return
   *     if no error was reported => std::monostate
   *     else => error code
   */
#if __has_include(<pthread.h>)
  constexpr std::variant<std::monostate, DetachError>
#else
  std::variant<std::monostate>
#endif
      detach();
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
  //! custom stack which is optinally allocated @see stackSize
  void* stack_{};
#endif
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
  const auto attr_init_result = ::pthread_attr_init(&ret.attr_);
  if (attr_init_result != 0) {
    return static_cast<ResourceError>(attr_init_result);
  }
  const auto set_inherit_sched_result =
      ::pthread_attr_setinheritsched(&ret.attr_, PTHREAD_EXPLICIT_SCHED);
  if (set_inherit_sched_result != 0) {
    return static_cast<InitError>(set_inherit_sched_result);
  }
#endif
  return ret;
}

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    typename Strand::PriorityType,
    typename Strand::GetPriorityError
> Strand::priority<Strand::Stage::NOT_LAUNCHED>() const {
  auto sched_param = ::sched_param{};
  const auto res = ::pthread_attr_getschedparam(&this->attr_, &sched_param);
  if (res != 0) {
    return static_cast<GetPriorityError>(res);
  }
  return sched_param.sched_priority;
}

template <>
constexpr std::variant<
    typename Strand::PriorityType,
    typename Strand::GetPriorityError
> Strand::priority<Strand::Stage::LAUNCHED>() const {
  auto sched_param = ::sched_param{};
  auto native_policy = int{};
  const auto res =
      ::pthread_getschedparam(this->thread_, &native_policy, &sched_param);
  if (res != 0) {
    return static_cast<GetPriorityError>(res);
  }
  return sched_param.sched_priority;
}
#endif

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    std::monostate,
    typename Strand::SetPriorityError
> Strand::priority<Strand::Stage::NOT_LAUNCHED>(const PriorityType& priority) {
  const auto sched_param = ::sched_param{ .sched_priority = priority, };
  const auto res = ::pthread_attr_setschedparam(
      &this->attr_,
      &sched_param
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
> Strand::priority<Strand::Stage::LAUNCHED>(const PriorityType& priority) {
  const auto res = ::pthread_setschedprio(this->thread_, priority);
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
  auto native_policy = int{};
  auto sched_param = ::sched_param{};
  const auto policy_get_result =
      ::pthread_attr_getschedpolicy(&this->attr_, &native_policy);
  if (policy_get_result != 0) {
    return static_cast<GetPolicyError>(policy_get_result);
  }
  const auto priority_get_result =
      ::pthread_attr_getschedparam(&this->attr_, &sched_param);
  if (priority_get_result != 0) {
    return static_cast<GetPriorityError>(priority_get_result);
  }
  return Scheduling{
    .policy = static_cast<Policy>(native_policy),
    .priority = sched_param.sched_priority,
  };
}

template <>
constexpr std::variant<
    typename Strand::Scheduling,
    typename Strand::GetPolicyError,
    typename Strand::GetPriorityError
> Strand::scheduling<Strand::Stage::LAUNCHED>() const {
  auto native_policy = int{};
  auto sched_param = ::sched_param{};
  const auto res =
      ::pthread_getschedparam(this->thread_, &native_policy, &sched_param);
  if (res != 0) {
    return static_cast<GetPolicyError>(res);
  }
  return Scheduling{
    .policy = static_cast<Policy>(native_policy),
    .priority = sched_param.sched_priority,
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
  const auto sched_param =
      ::sched_param{ .sched_priority = scheduling.priority, };
  const auto policy_set_result = ::pthread_attr_setschedpolicy(
      &this->attr_,
      static_cast<int>(scheduling.policy)
  );
  if (policy_set_result != 0) {
    return static_cast<SetPolicyError>(policy_set_result);
  }
  const auto priority_set_result = ::pthread_attr_setschedparam(
      &this->attr_,
      &sched_param
  );
  if (priority_set_result != 0) {
    return static_cast<SetPriorityError>(priority_set_result);
  }
  return std::monostate{};
}

template <>
constexpr std::variant<
    std::monostate,
    typename Strand::SetPolicyError,
    typename Strand::SetPriorityError
> Strand::scheduling<Strand::Stage::LAUNCHED>(const Scheduling& scheduling) {
  const auto sched_param =
      ::sched_param{ .sched_priority = scheduling.priority, };
  const auto res = ::pthread_setschedparam(
      this->thread_,
      static_cast<int>(scheduling.policy),
      &sched_param
  );
  if (res != 0) {
    return static_cast<SetPolicyError>(res);
  }
  return std::monostate{};
}
#endif

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    bool,
    typename Strand::GetDetachedError
> Strand::detached<Strand::Stage::NOT_LAUNCHED>() const {
  int detached;
  const auto detach_state_get_result = ::pthread_attr_getdetachstate(
      &this->attr_,
      &detached
  );
  if (detach_state_get_result != 0) {
    return static_cast<GetDetachedError>(detach_state_get_result);
  }
  return detached == PTHREAD_CREATE_DETACHED;
}
#endif

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    std::monostate,
    typename Strand::SetDetachedError
> Strand::detached<Strand::Stage::NOT_LAUNCHED>(
    const bool detached
) {
  const auto detach_state_set_result = ::pthread_attr_setdetachstate(
      &this->attr_,
      detached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE
  );
  if (detach_state_set_result != 0) {
    return static_cast<SetDetachedError>(detach_state_set_result);
  }
  return std::monostate{};
}
#endif

#if __has_include(<pthread.h>)
template <>
constexpr std::variant<
    std::size_t,
    typename Strand::GetStackSizeError
> Strand::stack_size<Strand::Stage::NOT_LAUNCHED>() const {
  void* stack_address;
  std::size_t stack_size;
  const auto& get_stack_result = ::pthread_attr_getstack(
      &this->attr_,
      &stack_address,
      &stack_size
  );
  if (get_stack_result != 0) {
    return static_cast<GetStackSizeError>(get_stack_result);
  }
  return stack_size;
}
#endif

#if __has_include(<pthread.h>)
#if __has_include(<stdlib.h>) && __has_include(<unistd.h>)
template <>
std::variant<
    std::monostate,
    typename Strand::SetStackSizeError,
    typename Strand::ResourceError
> Strand::allocate_stack<Strand::Stage::NOT_LAUNCHED>(
    const std::size_t stack_size
) {
  const auto allocate_result = ::posix_memalign(
      &this->stack_,
      sysconf(_SC_PAGE_SIZE),
      stack_size
  );
  if (allocate_result != 0) {
    return static_cast<ResourceError>(allocate_result);
  }
  const auto set_stack_result = ::pthread_attr_setstack(
      &this->attr_,
      this->stack_,
      stack_size
  );
  if (set_stack_result != 0) {
    return static_cast<SetStackSizeError>(set_stack_result);
  }
  return std::monostate{};
}
#endif
#endif

#if __has_include(<pthread.h>)
#if __has_include(<stdlib.h>) && __has_include(<unistd.h>)
template <>
std::variant<
    std::monostate
> Strand::deallocate_stack<Strand::Stage::TERMINATED>() {
  ::free(this->stack_);
  return std::monostate{};
}
#endif
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
  const auto error_code = ::pthread_create(
      &this->thread_,
      &this->attr_,
      &Strand::task,
      this
  );
  if (error_code != 0) {
    return static_cast<RunError>(error_code);
  }
  const auto attr_destroy_result = ::pthread_attr_destroy(&this->attr_);
  if (attr_destroy_result != 0) {
    return static_cast<ResourceError>(attr_destroy_result);
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
  const auto error_code = ::pthread_join(this->thread_, &res);
  if (error_code != 0) {
    return static_cast<JoinError>(error_code);
  }
#else
  this->thread_.join();
#endif
  return std::monostate{};
}

#if __has_include(<pthread.h>)
constexpr std::variant<
    std::monostate,
    typename Strand::DetachError
>
#else
std::variant<std::monostate>
#endif
Strand::detach() {
#if __has_include(<pthread.h>)
  const auto detach_result = ::pthread_detach(this->thread_);
  if (detach_result != 0) {
    return static_cast<DetachError>(detach_result);
  }
#else
  thread.detach();
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
