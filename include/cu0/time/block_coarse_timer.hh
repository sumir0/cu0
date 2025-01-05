#ifndef CU0_BLOCK_COARSE_TIMER_HH_
#define CU0_BLOCK_COARSE_TIMER_HH_

#include <chrono>
#include <thread>

namespace cu0 {

/*!
 * @brief struct representing blocking coarse timer which can be launched
 * @tparam Rep is the type representing the number of ticks @example float
 * @tparam Period is the type representing the tick period
 */
template <class Rep, class Period>
struct BlockCoarseTimer {
public:
  /*!
   * @brief constructs an instance with the specified duration
   * @param duration is the duration which should be waited after launch
   */
  explicit constexpr BlockCoarseTimer(
      std::chrono::duration<Rep, Period> duration
  );
  /*!
   * @brief launches the timer and sleeps the specified duration @see duration_
   */
  constexpr void launch() const;
//  constexpr void wait() const;
protected:
  //! duration to sleep and block after launch
  std::chrono::duration<Rep, Period> duration_;
//  std::chrono::time_point<
//      std::chrono::steady_clock,
//      std::chrono::duration<Rep, Period>
//  > launchTime_;
private:
};

} /// namespace cu0

namespace cu0 {

template <class Rep, class Period>
constexpr BlockCoarseTimer<Rep, Period>::BlockCoarseTimer(
    std::chrono::duration<Rep, Period> duration
) : duration_{std::move(duration)} {}

template <class Rep, class Period>
constexpr void BlockCoarseTimer<Rep, Period>::launch() const {
//  this->launchTime_ = std::chrono::time_point_cast<
//      std::chrono::duration<Rep, Period>
//  >(std::chrono::steady_clock::now());
  std::this_thread::sleep_for(this->duration_);
}

//template <class Rep, class Period>
//constexpr void CoarseTimer<Rep, Period>::wait() const {
//  std::this_thread::sleep_until(this->launchTime_ + this->duration_);
//}

} /// namespace cu0

#endif /// CU0_BLOCK_COARSE_TIMER_HH_
