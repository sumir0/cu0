#ifndef CU0_BLOCK_COARSE_TIMER_HH_
#define CU0_BLOCK_COARSE_TIMER_HH_

#include <chrono>
#include <thread>

namespace cu0 {

/*!
 * @brief struct representing blocking coarse timer which can be launched
 * @tparam Rep is the type representing the number of ticks @example float
 * @tparam Period is the type representing the tick period
 *     @example std::milli is the period of one millisecond
 *     @example std::ratio<1, 1> is the period of one second
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
protected:
  //! duration to sleep and block after launch
  std::chrono::duration<Rep, Period> duration_;
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
  std::this_thread::sleep_for(this->duration_);
}

} /// namespace cu0

#endif /// CU0_BLOCK_COARSE_TIMER_HH_
