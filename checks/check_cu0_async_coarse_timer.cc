#include <cu0/time/async_coarse_timer.hh>
#include <cassert>

#include <iostream>

int main() {
  for (auto i = 0; i < 1024; i++) {
    auto timer_2ms = cu0::AsyncCoarseTimer<double, std::milli>{
      std::chrono::duration<std::int64_t, std::milli>{2}
    };
    const auto start_2ms = std::chrono::steady_clock::now();
    timer_2ms.launch();
    timer_2ms.wait();
    const auto elapsed_2ms = std::chrono::duration_cast<
        std::chrono::duration<double, std::milli>
    >(std::chrono::steady_clock::now() - start_2ms);
    std::cout << elapsed_2ms << '\n';
    std::cout.flush();
    assert(elapsed_2ms.count() >= 2);
    assert(elapsed_2ms.count() < 8);
  }
  auto timer_2s = cu0::AsyncCoarseTimer<double, std::ratio<1, 1>>{
    std::chrono::duration<std::int64_t>{2}
  };
  const auto start_2s = std::chrono::steady_clock::now();
  timer_2s.launch();
  timer_2s.wait();
  const auto elapsed_2s = std::chrono::duration_cast<
      std::chrono::duration<double>
  >(std::chrono::steady_clock::now() - start_2s);
  std::cout << elapsed_2s << '\n';
  std::cout.flush();
  assert(elapsed_2s.count() >= 1);
  assert(elapsed_2s.count() < 3);
}
