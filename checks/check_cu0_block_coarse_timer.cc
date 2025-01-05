#include <cu0/time/block_coarse_timer.hh>
#include <cassert>

#include <iostream>

int main() {
  for (auto i = 0; i < 1024; i++) {
    auto timer2ms = cu0::BlockCoarseTimer<std::int64_t, std::milli>{
      std::chrono::duration<std::int64_t, std::milli>{2}
    };
    const auto start2ms = std::chrono::high_resolution_clock::now();
    timer2ms.launch();
  //  timer20ms.wait();
    const auto elapsed2ms = std::chrono::duration_cast<
        std::chrono::duration<double, std::milli>
    >(std::chrono::high_resolution_clock::now() - start2ms);
    std::cout << elapsed2ms << '\n';
    assert(elapsed2ms.count() >= 2);
    assert(elapsed2ms.count() < 8);
  }
  auto timer2s = cu0::BlockCoarseTimer<std::int64_t, std::ratio<1, 1>>{
    std::chrono::duration<std::int64_t>{2}
  };
  const auto start2s = std::chrono::high_resolution_clock::now();
  timer2s.launch();
//  timer2s.wait();
  const auto elapsed2s = std::chrono::duration_cast<
      std::chrono::duration<double>
  >(std::chrono::high_resolution_clock::now() - start2s);
  std::cout << elapsed2s << '\n';
  assert(elapsed2s.count() >= 2);
  assert(elapsed2s.count() < 3);
}
