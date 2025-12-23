#include <cu0/platform/not_an_x.hh>

int main() {
  constexpr auto IS_NOT_AN_X_DEFINED =
#if !defined(NOT_AN_X)
      false
#else
      true
#endif
      ;
  static_assert(
#if defined(__unix__)
      !IS_NOT_AN_X_DEFINED
#else
      IS_NOT_AN_X_DEFINED
#endif
  );
}
