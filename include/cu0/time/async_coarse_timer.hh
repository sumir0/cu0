#ifndef CU0_ASYNC_COARSE_TIMER_HH__
#define CU0_ASYNC_COARSE_TIMER_HH__

#include <chrono>
#include <thread>

#include <cu0/time/block_coarse_timer.hh>

namespace cu0 {

/*!
 * @brief struct representing asynchronous coarse timer which can be launched
 * @tparam Rep is the type representing number of ticks @example float
 * @tparam Period is the type representing tick period
 *     @example std::milli is the period of one millisecond
 *     @example std::ratio<1, 1> is the period of one second
 */
template <class Rep, class Period>
struct AsyncCoarseTimer : protected BlockCoarseTimer<Rep, Period> {
public:
  /*!
   * @brief constructs an instance with the specified duration
   * @param duration is the duration which should be waited after launch
   */
  explicit constexpr AsyncCoarseTimer(
      std::chrono::duration<Rep, Period> duration
  );
  /*!
   * @brief launches the timer @see wait()
   */
  constexpr void launch();
  /*!
   * @brief waits for the timer to be up if it is not already
   */
  constexpr void wait() const;
protected:
  //! time point when timer was launched
  std::chrono::time_point<
      std::chrono::steady_clock,
      std::chrono::duration<Rep, Period>
  > launch_time_;
private:
};

} /// namespace cu0

namespace cu0 {

template <class Rep, class Period>
constexpr AsyncCoarseTimer<Rep, Period>::AsyncCoarseTimer(
    std::chrono::duration<Rep, Period> duration
) : BlockCoarseTimer<Rep, Period>{std::move(duration)} {}

template <class Rep, class Period>
constexpr void AsyncCoarseTimer<Rep, Period>::launch() {
  this->launch_time_ = std::chrono::time_point_cast<
      std::chrono::duration<Rep, Period>
  >(std::chrono::steady_clock::now());
}

template <class Rep, class Period>
constexpr void AsyncCoarseTimer<Rep, Period>::wait() const {
  std::this_thread::sleep_until(
      this->launch_time_ + BlockCoarseTimer<Rep, Period>::duration_
  );
}

} /// namespace cu0

#endif /// CU0_ASYNC_COARSE_TIMER_HH__
