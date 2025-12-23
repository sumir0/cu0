#include <cu0/proc/executable.hh>
#include <filesystem>
#include <iostream>

int main() {
  //! @note executable can be searched in a directory
  const auto executable = cu0::util::find_by(
      "some_executable_name",
      std::filesystem::current_path()
  );
  //! @note if no executable is found then executable.binary is empty
  if (executable.binary.empty()) {
    std::cout << "Executable with the specified name was not found" << '\n';
  } else {
    //! @note if an executable is found then executable.binary is a path to it
    std::cout << "Path to the executable: " << executable.binary << '\n';
  }
}
