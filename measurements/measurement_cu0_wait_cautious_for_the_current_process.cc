#include <cu0/proc/process.hh>
#include <iostream>

#if \
  !__has_include(<sys/types.h>) || \
  !__has_include(<sys/wait.h>) || \
  !__has_include(<unistd.h>)
#warning <sys/types.h> or <sys/wait.h> or <unistd.h> is not found => \
    measurement_cu0_wait_cautious_for_the_current_process will be hollow
int main() {}
#else

int main() {
  auto thisProcess = cu0::Process::current();
  const auto result = thisProcess.waitCautious();
  std::string message;
  if (std::holds_alternative<std::monostate>(result)) {
    message = "std::monostate";
  } else {
    const auto errorCode = std::get<cu0::Process::WaitError>(result);
    switch (errorCode) {
    case cu0::Process::WaitError::CHILD:
      message = "cu0::Process::WaitError::CHILD";
      break;
    case cu0::Process::WaitError::INTR:
      message = "cu0::Process::WaitError::INTR";
      break;
    case cu0::Process::WaitError::INVAL:
      message = "cu0::Process::WaitError::INVAL";
      break;
    default:
      message = "cu0::Process::WaitError";
      message += '(';
      message += std::to_string(static_cast<int>(errorCode));
      message += ')';
      break;
    }
  }
  std::cout << message << '\n';
}

#endif
