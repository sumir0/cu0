#include <cu0/proc/strand.hh>
#include <iostream>

int main() {
  const auto variant = cu0::Strand::create([](){ return; });
  if (!std::holds_alternative<cu0::Strand>(variant)) {
    std::cerr << "Strand couldn't be created" << '\n';
  }
  const auto& strand = std::get<cu0::Strand>(variant);
}
