#include <cu0/proc/executable.hh>
#include <iostream>

int main() {
  //! @note executable can be searched in the current directory and
  //!     in the directories specified by the PATH environment variable
  const auto executable = cu0::util::findBy("someExecutableName");
  //! @note if no executable is found then executable.binary is empty
  if (executable.binary.empty()) {
    std::cout << "Executable with the specified name was not found" << '\n';
  } else {
    //! @note if an executable is found then executable.binary is a path to it
    std::cout << "Path to the executable: " << executable.binary << '\n';
  }
}
