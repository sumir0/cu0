#include <cu0/thread/thread_scheduling.hh>
#include <cassert>
#include <iostream>
#include <thread>
#ifdef __unix__
#if __has_include(<pthread.h>)
  #include <pthread.h>
#endif
#endif

int main() {
  {
    constexpr auto threadScheduling = cu0::ThreadScheduling<std::thread>{};
    static_assert(
        threadScheduling.nativeHandle() == std::thread::native_handle_type{}
    );
  }
  {
    constexpr auto threadScheduling = cu0::ThreadScheduling<std::thread>{1};
    static_assert(
        threadScheduling.nativeHandle() == std::thread::native_handle_type{1}
    );
  }
#ifdef __unix__
#if __has_include(<pthread.h>)
  int initialPriority = 0;
  {
    const auto threadScheduling =
        cu0::ThreadScheduling<std::thread>{pthread_self()};
    const auto [priority, status] = threadScheduling.priority();
    if (status == cu0::ThreadScheduling<std::thread>::GetPriorityStatus::DONE) {
      initialPriority = priority;
      std::cout << "This thread initial priority == " << initialPriority <<
          '\n';
    } else {
      std::cout << "This thread initial priority is unknown, assuming it ==" <<
          initialPriority << '\n';
    }
    std::cout.flush();
  }
#endif
#endif
#ifdef __unix__
#if __has_include(<pthread.h>)
  {
    const auto newPriority = initialPriority == 1 ? 2 : 1;
    auto threadScheduling =
        cu0::ThreadScheduling<std::thread>{pthread_self()};
    const auto setStatus = threadScheduling.priority(
        cu0::ThreadScheduling<std::thread>::Policy::UNIX_ROUND_ROBIN,
        newPriority
    );
    const auto [getPriority, getStatus] = threadScheduling.priority();
    assert(
        getStatus == cu0::ThreadScheduling<std::thread>::GetPriorityStatus::DONE
    );
    if (
        setStatus !=
            cu0::ThreadScheduling<std::thread>::SetPriorityStatus::DONE
    ) {
      assert(getPriority == initialPriority);
    } else {
      assert(getPriority == newPriority);
    }
  }
#endif
#endif

  return 0;
}
