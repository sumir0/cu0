#include <cu0/time/block_coarse_timer.hh>
#include <iostream>

int main() {
  //! create timer set for approximately 2 seconds
  auto timer = cu0::BlockCoarseTimer<std::int64_t, std::ratio<1, 1>>{
    std::chrono::duration<std::int64_t>{2}
  };
  //! measure start time
  const auto start = std::chrono::high_resolution_clock::now();
  //! launch timer
  timer.launch(); //! will block until the timer is up
  //! measure elapsed time
  const auto elapsed = std::chrono::duration_cast<
      std::chrono::duration<double>
  >(std::chrono::high_resolution_clock::now() - start);

  std::cout << "Elapsed: " << elapsed << '\n';
}
