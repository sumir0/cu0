#include <chrono>
#include <iostream>
#include <variant>

template <class Result, class Error>
constexpr std::variant<Result, Error> mock(
    [[maybe_unused]] const Result& result,
    const Error& error,
    [[maybe_unused]] const bool& error_flag
) {
  return std::variant<Result, Error>{error};
}

template <class Result, class Error>
constexpr std::variant<Result, Error> mock(
    const Result& result,
    [[maybe_unused]] const Error& error
) {
  return std::variant<Result, Error>{result};
}

template <class Result>
constexpr Result mock(
    const Result& result
) {
  return result;
}

template <class... Args>
constexpr std::chrono::nanoseconds measure(
    Args&&... args
) {
  const auto start = std::chrono::high_resolution_clock::now();
  const auto value = mock(args...);
  const auto end = std::chrono::high_resolution_clock::now();
  if (&value != nullptr) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  }
  return {};
}

int main() {
  constexpr auto N = 2 << 13;
  std::array<std::size_t, 3> mean{};
  for (auto& v : mean) {
    v = 0;
  }
  for (auto i = 0; i < N; i++) {
    mean[0] += measure(std::string{"measure1"} + std::to_string(i)).count();
    mean[1] +=
        measure(std::string{"measure2"} + std::to_string(i), int{0}).count();
    mean[2] +=
        measure(std::string{"measure3"} + std::to_string(i), int{1}, false).
        count();
  }
  std::cout << "std::string: " << static_cast<double>(mean[0]) / N <<
      "ns" << '\n';
  std::cout << "std::variant<std::string, int> (0): " <<
      static_cast<double>(mean[1]) / N << "ns" << '\n';
  std::cout << "std::variant<std::string, int> (1): " <<
      static_cast<double>(mean[2]) / N << "ns" << '\n';
}
