#include <cu0/proc/process.hh>
#include <iostream>

#ifndef __unix__
#warning __unix__ is not defined => \
    measurement_cu0_wait_cautious_for_the_current_process will be hollow
int main() {}
#else
#if \
  !__has_include(<sys/types.h>) || \
  !__has_include(<sys/wait.h>) || \
  !__has_include(<unistd.h>)
#warning <sys/types.h> or <sys/wait.h> is not found => \
    measurement_cu0_wait_cautious_for_the_current_process will be hollow
int main() {}
#else

int main() {
  auto thisProcess = cu0::Process::current();
  const auto errorCode = thisProcess.waitCautious();
  std::string error;
  switch (errorCode) {
  case cu0::Process::WaitError::NO_ERROR:
    error = "cu0::Process::WaitError::NO_ERROR";
    break;
  case cu0::Process::WaitError::CHILD:
    error = "cu0::Process::WaitError::CHILD";
    break;
  case cu0::Process::WaitError::INTR:
    error = "cu0::Process::WaitError::INTR";
    break;
  case cu0::Process::WaitError::INVAL:
    error = "cu0::Process::WaitError::INVAL";
    break;
  default:
    error = "cu0::Process::WaitError";
    error += '(';
    error += std::to_string(static_cast<int>(errorCode));
    error += ')';
    break;
  }
  std::cout << error << '\n';
}

#endif
#endif
