#include <cu0/time/block_coarse_timer.hh>
#include <cassert>

#include <iostream>

int main() {
  for (auto i = 0; i < 1024; i++) {
    constexpr auto timer2ms = cu0::BlockCoarseTimer<double, std::milli>{
      std::chrono::duration<std::int64_t, std::milli>{2}
    };
    const auto start2ms = std::chrono::steady_clock::now();
    timer2ms.launch();
    const auto elapsed2ms = std::chrono::duration_cast<
        std::chrono::duration<double, std::milli>
    >(std::chrono::steady_clock::now() - start2ms);
    std::cout << elapsed2ms << '\n';
    std::cout.flush();
    assert(elapsed2ms.count() >= 2);
    assert(elapsed2ms.count() < 8);
  }
  constexpr auto timer2s = cu0::BlockCoarseTimer<double, std::ratio<1, 1>>{
    std::chrono::duration<std::int64_t>{2}
  };
  const auto start2s = std::chrono::steady_clock::now();
  timer2s.launch();
  const auto elapsed2s = std::chrono::duration_cast<
      std::chrono::duration<double>
  >(std::chrono::steady_clock::now() - start2s);
  std::cout << elapsed2s << '\n';
  std::cout.flush();
  assert(elapsed2s.count() >= 2);
  assert(elapsed2s.count() < 3);
}
