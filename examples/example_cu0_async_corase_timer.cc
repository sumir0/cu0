#include <cu0/time/async_coarse_timer.hh>
#include <iostream>

int main() {
  //! create timer set for approximately 2000 milliseconds
  auto timer = cu0::AsyncCoarseTimer<std::int64_t, std::milli>{
    std::chrono::duration<std::int64_t, std::milli>{2000}
  };
  //! measure start time
  const auto start = std::chrono::high_resolution_clock::now();
  //! launch the timer
  timer.launch();
  //! wait for the timer
  timer.wait(); //! will block until the timer is up
  //! measure elapsed time
  const auto elapsed = std::chrono::duration_cast<
      std::chrono::duration<double>
  >(std::chrono::high_resolution_clock::now() - start);

  std::cout << "Elapsed: " << elapsed << '\n';
}
