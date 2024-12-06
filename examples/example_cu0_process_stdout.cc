#include <cu0/proc.hxx>
#include <iostream>

//! @note supported features may vary on different platforms
//! @note
//!     if some feature is not supported ->
//!         a compile-time warning will be present
//!     else (if all features are supported) ->
//!         no feature-related compile-time warnings will be present
#ifndef __unix__
#warning __unix__ is not defined => \
    cu0::Process::stdout() will not be used in the example
int main() {}
#else
#if !__has_include(<unistd.h>)
#warning <unistd.h> is not found => \
    cu0::Process::stdout() will not be used in the example
int main() {}
#else

int main() {
  auto someProcess = cu0::Process::create(cu0::Executable{
    .binary = "someExecutable"
  });
  if (!someProcess.has_value()) {
    std::cout << "Error: the process was not created" << '\n';
  }
  //! @note not supported on all platforms yet
  //! @note stdout contains standard output of the created process
  //!     at the moment of call
  const auto output = someProcess->stdout();
  if (output.str().empty()) {
    std::cout << "Stdout of the created process is empty" << '\n';
  } else {
    std::cout << "Stdout of the created process: " << output.str() << '\n';
  }
}

#endif
#endif
