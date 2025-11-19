#ifndef CU0_EXECUTABLE_HH__
#define CU0_EXECUTABLE_HH__

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <cu0/env/environment_variable.hh>

namespace cu0 {

struct Executable {
  std::filesystem::path binary{};
  std::vector<std::string> arguments{};
  std::map<std::string, std::string> environment{};
};

namespace util {

/*!
 * @brief finds an executable by a name
 * @note searches in order:
 *     the current directory,
 *     the directories specified by the PATH environment variable
 * @param name is the name of an executable
 * @return executable with empty arguments and an empty environment
 * @note if multiple executables are present with the specified name =>
 *     the first found executable is returned
 */
[[nodiscard]]
Executable findBy(const std::string& name);

/*!
 * @brief finds an executable by a name
 * @param name is the name of an executable
 * @param directory is the directory where to look for an executable
 * @return executable with empty arguments and an empty environment
 */
[[nodiscard]]
Executable findBy(
    const std::string& name,
    const std::filesystem::path& directory
);

/*!
 * @brief converts arguments of an executable to ptr<ptr<char[]>[]>
 * @param executable is the executable, arguments of which will be converted
 * @return tuple containing a ptr to an array and the size of that array
 */
[[nodiscard]]
std::tuple<std::unique_ptr<std::unique_ptr<char[]>[]>, std::size_t> argvOf(
    const Executable& executable
);

/*!
 * @brief converts environment of an executable to ptr<ptr<char[]>[]>
 *     @note in format of "key=value"
 * @param executable is the executable, environment of which will be converted
 * @return tuple containing a ptr to an array and the size of that array
 */
[[nodiscard]]
std::tuple<std::unique_ptr<std::unique_ptr<char[]>[]>, std::size_t> envpOf(
    const Executable& executable
);

} /// namespace util

} /// namespace cu0

namespace cu0 {

namespace util {

inline Executable findBy(const std::string& name) {
  auto candidate = findBy(name, std::filesystem::current_path());
  if (!candidate.binary.empty()) {
    return candidate;
  }
  const auto environmentVariable = EnvironmentVariable::synced("PATH");
  const auto& optional = environmentVariable.cached();
  if (!optional.has_value()) {
    return {};
  }
  auto pathsLeft = optional.value();
  if (pathsLeft.empty()) {
    return {};
  }
  while (true) {
#ifndef not_an_x
    const auto pos = pathsLeft.find(':');
#else
    const auto pos = pathsLeft.find(';');
#endif
    const auto path = pathsLeft.substr(0, pos);
    if (
        std::filesystem::is_directory(path) &&
        !(candidate = findBy(name, path)).binary.empty()
    ) {
      return candidate;
    }
    if (pos == std::string::npos) {
      break;
    }
    /// else
    pathsLeft.erase(0, pos + 1);
  }
  return {};
}

inline Executable findBy(
    const std::string& name,
    const std::filesystem::path& directory
) {
  for (const auto& it : std::filesystem::directory_iterator{directory}) {
    if (it.path().filename() == name) {
      return { .binary = it.path() };
    }
  }
  return {};
}

inline std::tuple<std::unique_ptr<std::unique_ptr<char[]>[]>, std::size_t>
argvOf(
    const Executable& executable
) {
  //! argv == executable.binary + executable.arguments + NULL
  const auto size = 1 + executable.arguments.size() + 1;
  auto argv = std::make_unique<std::unique_ptr<char[]>[]>(size);
  argv[0] = std::make_unique<char[]>(executable.binary.string().size() + 1);
  for (auto j = 0u; j < executable.binary.string().size(); j++) {
    argv[0][j] = executable.binary.string()[j];
  }
  argv[0][executable.binary.string().size()] = '\0';
  for (auto i = 1u; i < size - 1; i++) {
    argv[i] = std::make_unique<char[]>(executable.arguments[i - 1].size() + 1);
    for (auto j = 0u; j < executable.arguments[i - 1].size(); j++) {
      argv[i][j] = executable.arguments[i - 1][j];
    }
    argv[i][executable.arguments[i - 1].size()] = '\0';
  }
  argv[size - 1] = NULL;
  return std::make_tuple(std::move(argv), size);
}

inline std::tuple<std::unique_ptr<std::unique_ptr<char[]>[]>, std::size_t>
envpOf(
    const Executable& executable
) {
  //! envp == formatted(executable.environment) + NULL
  const auto size = executable.environment.size() + 1;
  auto envp = std::make_unique<std::unique_ptr<char[]>[]>(size);
  auto i = 0;
  for (const auto& [key, value] : executable.environment) {
    const auto string = key + "=" + value;
    envp[i] = std::make_unique<char[]>(string.size() + 1);
    for (auto j = 0u; j < string.size(); j++) {
      envp[i][j] = string[j];
    }
    envp[i][string.size()] = '\0';
    i++;
  }
  envp[size - 1] = NULL;
  return std::make_tuple(std::move(envp), size);
}

} /// namespace util

} /// namespace cu0

#endif /// CU0_EXECUTABLE_HH__
