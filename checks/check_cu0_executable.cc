#include <cu0/proc/executable.hh>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <cu0/platform/not_an_x.hh>

int main() {

  const auto name = std::string{"name"};
  auto i = 0;
  auto unique_name = name;
  while (std::filesystem::exists(unique_name)) {
    unique_name = name + std::to_string(++i);
  }
  std::ofstream{unique_name};

  const auto unique_executable_in_the_current_directory =
      cu0::util::find_by(unique_name, std::filesystem::current_path());
  assert(
      unique_executable_in_the_current_directory.binary ==
          std::filesystem::current_path() / unique_name
  );
  assert(unique_executable_in_the_current_directory.arguments.empty());
  assert(unique_executable_in_the_current_directory.environment.empty());

  const auto unique_executable = cu0::util::find_by(unique_name);
  assert(
      unique_executable.binary == std::filesystem::current_path() / unique_name
  );
  assert(unique_executable.arguments.empty());
  assert(unique_executable.environment.empty());

  std::filesystem::remove(unique_name);

  const auto removed_executable_in_the_current_directory =
      cu0::util::find_by(unique_name, std::filesystem::current_path());
  assert(removed_executable_in_the_current_directory.binary.empty());
  assert(removed_executable_in_the_current_directory.arguments.empty());
  assert(removed_executable_in_the_current_directory.environment.empty());

  const auto removed_executable = cu0::util::find_by(unique_name);
  assert(removed_executable.binary.empty());
  assert(unique_executable.arguments.empty());
  assert(unique_executable.environment.empty());

#if !defined(NOT_AN_X)
  const auto bin = std::filesystem::path{"/usr/bin"};
  const auto path_environment_variable =
      cu0::EnvironmentVariable::synced("PATH");
  const auto path = path_environment_variable.cached().has_value() ?
      path_environment_variable.cached().value() :
      std::string{};

  const auto ls = bin / "ls";
  if (
      std::filesystem::exists(ls) &&
      path.find(bin.string()) != std::string::npos
  ) {
    const auto ls_executable = cu0::util::find_by(ls.filename());
    assert(ls_executable.binary == ls);
    assert(ls_executable.arguments.empty());
    assert(ls_executable.environment.empty());
  }

  const auto sh = bin / "sh";
  if (
      std::filesystem::exists(sh) &&
      path.find(bin.string()) != std::string::npos
  ) {
    const auto sh_executable = cu0::util::find_by(sh.filename());
    assert(sh_executable.binary == sh);
    assert(sh_executable.arguments.empty());
    assert(sh_executable.environment.empty());
  }
#endif

  const auto executable = cu0::Executable{};

  const auto argv_of_executable = cu0::util::argv_of(executable);
  assert(std::get<1>(argv_of_executable) == 2);
  assert(std::get<0>(argv_of_executable)[0] != NULL);
  assert(std::get<0>(argv_of_executable)[0][0] == '\0');
  assert(std::get<0>(argv_of_executable)[1] == NULL);

  const auto envp_of_executable = cu0::util::envp_of(executable);
  assert(std::get<1>(envp_of_executable) == 1);
  assert(std::get<0>(envp_of_executable)[0] == NULL);

  const auto executable_with_binary = cu0::Executable{ .binary = "binary" };

  const auto argv_of_executable_with_binary =
      cu0::util::argv_of(executable_with_binary);
  assert(std::get<1>(argv_of_executable_with_binary) == 2);
  assert(std::get<0>(argv_of_executable_with_binary)[0] != NULL);
  assert(std::get<0>(argv_of_executable_with_binary)[0][0] == 'b');
  assert(std::get<0>(argv_of_executable_with_binary)[0][1] == 'i');
  assert(std::get<0>(argv_of_executable_with_binary)[0][2] == 'n');
  assert(std::get<0>(argv_of_executable_with_binary)[0][3] == 'a');
  assert(std::get<0>(argv_of_executable_with_binary)[0][4] == 'r');
  assert(std::get<0>(argv_of_executable_with_binary)[0][5] == 'y');
  assert(std::get<0>(argv_of_executable_with_binary)[0][6] == '\0');
  assert(std::get<0>(argv_of_executable_with_binary)[1] == NULL);

  const auto envp_of_executable_with_binary =
      cu0::util::envp_of(executable_with_binary);
  assert(std::get<1>(envp_of_executable_with_binary) == 1);
  assert(std::get<0>(envp_of_executable_with_binary)[0] == NULL);

  const auto executable_with_arguments =
      cu0::Executable{ .arguments = {"arg1"} };

  const auto argv_of_executable_with_arguments =
      cu0::util::argv_of(executable_with_arguments);
  assert(std::get<1>(argv_of_executable_with_arguments) == 3);
  assert(std::get<0>(argv_of_executable_with_arguments)[0] != NULL);
  assert(std::get<0>(argv_of_executable_with_arguments)[0][0] == '\0');
  assert(std::get<0>(argv_of_executable_with_arguments)[1] != NULL);
  assert(std::get<0>(argv_of_executable_with_arguments)[1][0] == 'a');
  assert(std::get<0>(argv_of_executable_with_arguments)[1][1] == 'r');
  assert(std::get<0>(argv_of_executable_with_arguments)[1][2] == 'g');
  assert(std::get<0>(argv_of_executable_with_arguments)[1][3] == '1');
  assert(std::get<0>(argv_of_executable_with_arguments)[1][4] == '\0');
  assert(std::get<0>(argv_of_executable_with_arguments)[2] == NULL);

  const auto envp_of_executable_with_arguments =
      cu0::util::envp_of(executable_with_arguments);
  assert(std::get<1>(envp_of_executable_with_arguments) == 1);
  assert(std::get<0>(envp_of_executable_with_arguments)[0] == NULL);

  const auto executable_with_environment =
      cu0::Executable{ .environment = { { "key", "value", } } };

  const auto argv_of_executable_with_environment =
      cu0::util::argv_of(executable_with_environment);
  assert(std::get<1>(argv_of_executable_with_environment) == 2);
  assert(std::get<0>(argv_of_executable_with_environment)[0] != NULL);
  assert(std::get<0>(argv_of_executable_with_environment)[0][0] == '\0');
  assert(std::get<0>(argv_of_executable_with_environment)[1] == NULL);

  const auto envp_of_executable_with_environment =
      cu0::util::envp_of(executable_with_environment);
  assert(std::get<1>(envp_of_executable_with_environment) == 2);
  assert(std::get<0>(envp_of_executable_with_environment)[0] != NULL);
  assert(std::get<0>(envp_of_executable_with_environment)[0][0] == 'k');
  assert(std::get<0>(envp_of_executable_with_environment)[0][1] == 'e');
  assert(std::get<0>(envp_of_executable_with_environment)[0][2] == 'y');
  assert(std::get<0>(envp_of_executable_with_environment)[0][3] == '=');
  assert(std::get<0>(envp_of_executable_with_environment)[0][4] == 'v');
  assert(std::get<0>(envp_of_executable_with_environment)[0][5] == 'a');
  assert(std::get<0>(envp_of_executable_with_environment)[0][6] == 'l');
  assert(std::get<0>(envp_of_executable_with_environment)[0][7] == 'u');
  assert(std::get<0>(envp_of_executable_with_environment)[0][8] == 'e');
  assert(std::get<0>(envp_of_executable_with_environment)[0][9] == '\0');
  assert(std::get<0>(envp_of_executable_with_environment)[1] == NULL);

  const auto executable_with_arguments_and_environment = cu0::Executable{
    .arguments = {"arg1"},
    .environment = { { "key", "value", } },
  };

  const auto argv_of_executable_with_arguments_and_environment =
      cu0::util::argv_of(executable_with_arguments_and_environment);
  const auto& argv_of_executable_with_arguments_and_environment_data =
      std::get<0>(argv_of_executable_with_arguments_and_environment);
  assert(std::get<1>(argv_of_executable_with_arguments_and_environment) == 3);
  assert(argv_of_executable_with_arguments_and_environment_data[0] != NULL);
  assert(argv_of_executable_with_arguments_and_environment_data[0][0] == '\0');
  assert(argv_of_executable_with_arguments_and_environment_data[1] != NULL);
  assert(argv_of_executable_with_arguments_and_environment_data[1][0] == 'a');
  assert(argv_of_executable_with_arguments_and_environment_data[1][1] == 'r');
  assert(argv_of_executable_with_arguments_and_environment_data[1][2] == 'g');
  assert(argv_of_executable_with_arguments_and_environment_data[1][3] == '1');
  assert(argv_of_executable_with_arguments_and_environment_data[1][4] == '\0');
  assert(argv_of_executable_with_arguments_and_environment_data[2] == NULL);

  const auto envp_of_executable_with_arguments_and_environment =
      cu0::util::envp_of(executable_with_environment);
  const auto& envp_of_executable_with_arguments_and_environment_data =
      std::get<0>(envp_of_executable_with_arguments_and_environment);
  assert(std::get<1>(envp_of_executable_with_arguments_and_environment) == 2);
  assert(envp_of_executable_with_arguments_and_environment_data[0] != NULL);
  assert(envp_of_executable_with_arguments_and_environment_data[0][0] == 'k');
  assert(envp_of_executable_with_arguments_and_environment_data[0][1] == 'e');
  assert(envp_of_executable_with_arguments_and_environment_data[0][2] == 'y');
  assert(envp_of_executable_with_arguments_and_environment_data[0][3] == '=');
  assert(envp_of_executable_with_arguments_and_environment_data[0][4] == 'v');
  assert(envp_of_executable_with_arguments_and_environment_data[0][5] == 'a');
  assert(envp_of_executable_with_arguments_and_environment_data[0][6] == 'l');
  assert(envp_of_executable_with_arguments_and_environment_data[0][7] == 'u');
  assert(envp_of_executable_with_arguments_and_environment_data[0][8] == 'e');
  assert(envp_of_executable_with_arguments_and_environment_data[0][9] == '\0');
  assert(envp_of_executable_with_arguments_and_environment_data[1] == NULL);

  const auto executable_with_all_fields = cu0::Executable{
    .binary = "binary",
    .arguments = {"arg1"},
    .environment = { { "key", "value", } },
  };

  const auto argv_of_executable_with_all_fields =
      cu0::util::argv_of(executable_with_all_fields);
  assert(std::get<1>(argv_of_executable_with_all_fields) == 3);
  assert(std::get<0>(argv_of_executable_with_all_fields)[0] != NULL);
  assert(std::get<0>(argv_of_executable_with_all_fields)[0][0] == 'b');
  assert(std::get<0>(argv_of_executable_with_all_fields)[0][1] == 'i');
  assert(std::get<0>(argv_of_executable_with_all_fields)[0][2] == 'n');
  assert(std::get<0>(argv_of_executable_with_all_fields)[0][3] == 'a');
  assert(std::get<0>(argv_of_executable_with_all_fields)[0][4] == 'r');
  assert(std::get<0>(argv_of_executable_with_all_fields)[0][5] == 'y');
  assert(std::get<0>(argv_of_executable_with_all_fields)[0][6] == '\0');
  assert(std::get<0>(argv_of_executable_with_all_fields)[1] != NULL);
  assert(std::get<0>(argv_of_executable_with_all_fields)[1][0] == 'a');
  assert(std::get<0>(argv_of_executable_with_all_fields)[1][1] == 'r');
  assert(std::get<0>(argv_of_executable_with_all_fields)[1][2] == 'g');
  assert(std::get<0>(argv_of_executable_with_all_fields)[1][3] == '1');
  assert(std::get<0>(argv_of_executable_with_all_fields)[1][4] == '\0');
  assert(std::get<0>(argv_of_executable_with_all_fields)[2] == NULL);

  const auto envp_of_executable_with_all_fields =
      cu0::util::envp_of(executable_with_all_fields);
  assert(std::get<1>(envp_of_executable_with_all_fields) == 2);
  assert(std::get<0>(envp_of_executable_with_all_fields)[0] != NULL);
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][0] == 'k');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][1] == 'e');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][2] == 'y');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][3] == '=');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][4] == 'v');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][5] == 'a');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][6] == 'l');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][7] == 'u');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][8] == 'e');
  assert(std::get<0>(envp_of_executable_with_all_fields)[0][9] == '\0');
  assert(std::get<0>(envp_of_executable_with_all_fields)[1] == NULL);

  const auto executable_with_two_arguments = cu0::Executable{
    .binary = "someBinary",
    .arguments = { "arg1", "arg2", },
    .environment = { { "key", "value", } },
  };

  const auto argv_of_executable_with_two_arguments =
      cu0::util::argv_of(executable_with_two_arguments);
  assert(std::get<1>(argv_of_executable_with_two_arguments) == 4);
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0] != NULL);
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][0] == 's');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][1] == 'o');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][2] == 'm');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][3] == 'e');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][4] == 'B');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][5] == 'i');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][6] == 'n');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][7] == 'a');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][8] == 'r');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][9] == 'y');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[0][10] == '\0');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[1] != NULL);
  assert(std::get<0>(argv_of_executable_with_two_arguments)[1][0] == 'a');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[1][1] == 'r');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[1][2] == 'g');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[1][3] == '1');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[1][4] == '\0');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[2] != NULL);
  assert(std::get<0>(argv_of_executable_with_two_arguments)[2][0] == 'a');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[2][1] == 'r');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[2][2] == 'g');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[2][3] == '2');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[2][4] == '\0');
  assert(std::get<0>(argv_of_executable_with_two_arguments)[3] == NULL);

  const auto envp_of_executable_with_two_arguments =
      cu0::util::envp_of(executable_with_two_arguments);
  assert(std::get<1>(envp_of_executable_with_two_arguments) == 2);
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0] != NULL);
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][0] == 'k');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][1] == 'e');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][2] == 'y');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][3] == '=');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][4] == 'v');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][5] == 'a');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][6] == 'l');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][7] == 'u');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][8] == 'e');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[0][9] == '\0');
  assert(std::get<0>(envp_of_executable_with_two_arguments)[1] == NULL);

  const auto executable_with_two_environment_vars = cu0::Executable{
    .binary = "binary1",
    .arguments = {"arg1"},
    .environment = { { "key1", "value1", }, { "key2", "value2", } },
  };

  const auto argv_of_executable_with_two_environment_vars =
      cu0::util::argv_of(executable_with_two_environment_vars);
  assert(std::get<1>(argv_of_executable_with_two_environment_vars) == 3);
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[0] != NULL
  );
  assert(std::get<0>(
      argv_of_executable_with_two_environment_vars)[0][0] == 'b'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[0][1] == 'i'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[0][2] == 'n'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[0][3] == 'a'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[0][4] == 'r'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[0][5] == 'y'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[0][6] == '1'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[0][7] == '\0'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[1] != NULL
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[1][0] == 'a'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[1][1] == 'r'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[1][2] == 'g'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[1][3] == '1'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[1][4] == '\0'
  );
  assert(
      std::get<0>(argv_of_executable_with_two_environment_vars)[2] == NULL
  );

  const auto envp_of_executable_with_two_environment_vars =
      cu0::util::envp_of(executable_with_two_environment_vars);
  assert(std::get<1>(envp_of_executable_with_two_environment_vars) == 3);
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0] != NULL
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][0] == 'k'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][1] == 'e'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][2] == 'y'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][3] == '1'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][4] == '='
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][5] == 'v'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][6] == 'a'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][7] == 'l'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][8] == 'u'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][9] == 'e'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][10] == '1'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[0][11] == '\0'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1] != NULL
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][0] == 'k'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][1] == 'e'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][2] == 'y'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][3] == '2'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][4] == '='
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][5] == 'v'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][6] == 'a'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][7] == 'l'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][8] == 'u'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][9] == 'e'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][10] == '2'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[1][11] == '\0'
  );
  assert(
      std::get<0>(envp_of_executable_with_two_environment_vars)[2] == NULL
  );

  return 0;
}
