#include <cu0/proc/executable.hh>
#include <cassert>

int main() {

  const auto executable = cu0::Executable{};

  const auto argvOfExecutable = cu0::util::argvOf(executable);
  assert(std::get<1>(argvOfExecutable) == 2);
  assert(std::get<0>(argvOfExecutable)[0] != NULL);
  assert(std::get<0>(argvOfExecutable)[0][0] == '\0');
  assert(std::get<0>(argvOfExecutable)[1] == NULL);

  const auto envpOfExecutable = cu0::util::envpOf(executable);
  assert(std::get<1>(envpOfExecutable) == 1);
  assert(std::get<0>(envpOfExecutable)[0] == NULL);

  const auto executableWithBinary = cu0::Executable{ .binary = "binary" };

  const auto argvOfExecutableWithBinary =
      cu0::util::argvOf(executableWithBinary);
  assert(std::get<1>(argvOfExecutableWithBinary) == 2);
  assert(std::get<0>(argvOfExecutableWithBinary)[0] != NULL);
  assert(std::get<0>(argvOfExecutableWithBinary)[0][0] == 'b');
  assert(std::get<0>(argvOfExecutableWithBinary)[0][1] == 'i');
  assert(std::get<0>(argvOfExecutableWithBinary)[0][2] == 'n');
  assert(std::get<0>(argvOfExecutableWithBinary)[0][3] == 'a');
  assert(std::get<0>(argvOfExecutableWithBinary)[0][4] == 'r');
  assert(std::get<0>(argvOfExecutableWithBinary)[0][5] == 'y');
  assert(std::get<0>(argvOfExecutableWithBinary)[0][6] == '\0');
  assert(std::get<0>(argvOfExecutableWithBinary)[1] == NULL);

  const auto envpOfExecutableWithBinary =
      cu0::util::envpOf(executableWithBinary);
  assert(std::get<1>(envpOfExecutableWithBinary) == 1);
  assert(std::get<0>(envpOfExecutableWithBinary)[0] == NULL);

  const auto executableWithArguments =
      cu0::Executable{ .arguments = {"arg1"} };

  const auto argvOfExecutableWithArguments =
      cu0::util::argvOf(executableWithArguments);
  assert(std::get<1>(argvOfExecutableWithArguments) == 3);
  assert(std::get<0>(argvOfExecutableWithArguments)[0] != NULL);
  assert(std::get<0>(argvOfExecutableWithArguments)[0][0] == '\0');
  assert(std::get<0>(argvOfExecutableWithArguments)[1] != NULL);
  assert(std::get<0>(argvOfExecutableWithArguments)[1][0] == 'a');
  assert(std::get<0>(argvOfExecutableWithArguments)[1][1] == 'r');
  assert(std::get<0>(argvOfExecutableWithArguments)[1][2] == 'g');
  assert(std::get<0>(argvOfExecutableWithArguments)[1][3] == '1');
  assert(std::get<0>(argvOfExecutableWithArguments)[1][4] == '\0');
  assert(std::get<0>(argvOfExecutableWithArguments)[2] == NULL);

  const auto envpOfExecutableWithArguments =
      cu0::util::envpOf(executableWithArguments);
  assert(std::get<1>(envpOfExecutableWithArguments) == 1);
  assert(std::get<0>(envpOfExecutableWithArguments)[0] == NULL);

  const auto executableWithEnvironment =
      cu0::Executable{ .environment = { { "key", "value", } } };

  const auto argvOfExecutableWithEnvironment =
      cu0::util::argvOf(executableWithEnvironment);
  assert(std::get<1>(argvOfExecutableWithEnvironment) == 2);
  assert(std::get<0>(argvOfExecutableWithEnvironment)[0] != NULL);
  assert(std::get<0>(argvOfExecutableWithEnvironment)[0][0] == '\0');
  assert(std::get<0>(argvOfExecutableWithEnvironment)[1] == NULL);

  const auto envpOfExecutableWithEnvironment =
      cu0::util::envpOf(executableWithEnvironment);
  assert(std::get<1>(envpOfExecutableWithEnvironment) == 2);
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0] != NULL);
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][0] == 'k');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][1] == 'e');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][2] == 'y');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][3] == '=');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][4] == 'v');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][5] == 'a');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][6] == 'l');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][7] == 'u');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][8] == 'e');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[0][9] == '\0');
  assert(std::get<0>(envpOfExecutableWithEnvironment)[1] == NULL);

  const auto executableWithArgumentsAndEnvironment = cu0::Executable{
    .arguments = {"arg1"},
    .environment = { { "key", "value", } },
  };

  const auto argvOfExecutableWithArgumentsAndEnvironment =
      cu0::util::argvOf(executableWithArgumentsAndEnvironment);
  const auto& argvOfExecutableWithArgumentsAndEnvironmentData =
      std::get<0>(argvOfExecutableWithArgumentsAndEnvironment);
  assert(std::get<1>(argvOfExecutableWithArgumentsAndEnvironment) == 3);
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[0] != NULL);
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[0][0] == '\0');
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[1] != NULL);
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[1][0] == 'a');
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[1][1] == 'r');
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[1][2] == 'g');
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[1][3] == '1');
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[1][4] == '\0');
  assert(argvOfExecutableWithArgumentsAndEnvironmentData[2] == NULL);

  const auto envpOfExecutableWithArgumentsAndEnvironment =
      cu0::util::envpOf(executableWithEnvironment);
  const auto& envpOfExecutableWithArgumentsAndEnvironmentData =
      std::get<0>(envpOfExecutableWithArgumentsAndEnvironment);
  assert(std::get<1>(envpOfExecutableWithArgumentsAndEnvironment) == 2);
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0] != NULL);
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][0] == 'k');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][1] == 'e');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][2] == 'y');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][3] == '=');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][4] == 'v');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][5] == 'a');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][6] == 'l');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][7] == 'u');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][8] == 'e');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[0][9] == '\0');
  assert(envpOfExecutableWithArgumentsAndEnvironmentData[1] == NULL);

  const auto executableWithAllFields = cu0::Executable{
    .binary = "binary",
    .arguments = {"arg1"},
    .environment = { { "key", "value", } },
  };

  const auto argvOfExecutableWithAllFields =
      cu0::util::argvOf(executableWithAllFields);
  assert(std::get<1>(argvOfExecutableWithAllFields) == 3);
  assert(std::get<0>(argvOfExecutableWithAllFields)[0] != NULL);
  assert(std::get<0>(argvOfExecutableWithAllFields)[0][0] == 'b');
  assert(std::get<0>(argvOfExecutableWithAllFields)[0][1] == 'i');
  assert(std::get<0>(argvOfExecutableWithAllFields)[0][2] == 'n');
  assert(std::get<0>(argvOfExecutableWithAllFields)[0][3] == 'a');
  assert(std::get<0>(argvOfExecutableWithAllFields)[0][4] == 'r');
  assert(std::get<0>(argvOfExecutableWithAllFields)[0][5] == 'y');
  assert(std::get<0>(argvOfExecutableWithAllFields)[0][6] == '\0');
  assert(std::get<0>(argvOfExecutableWithAllFields)[1] != NULL);
  assert(std::get<0>(argvOfExecutableWithAllFields)[1][0] == 'a');
  assert(std::get<0>(argvOfExecutableWithAllFields)[1][1] == 'r');
  assert(std::get<0>(argvOfExecutableWithAllFields)[1][2] == 'g');
  assert(std::get<0>(argvOfExecutableWithAllFields)[1][3] == '1');
  assert(std::get<0>(argvOfExecutableWithAllFields)[1][4] == '\0');
  assert(std::get<0>(argvOfExecutableWithAllFields)[2] == NULL);

  const auto envpOfExecutableWithAllFields =
      cu0::util::envpOf(executableWithAllFields);
  assert(std::get<1>(envpOfExecutableWithAllFields) == 2);
  assert(std::get<0>(envpOfExecutableWithAllFields)[0] != NULL);
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][0] == 'k');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][1] == 'e');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][2] == 'y');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][3] == '=');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][4] == 'v');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][5] == 'a');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][6] == 'l');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][7] == 'u');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][8] == 'e');
  assert(std::get<0>(envpOfExecutableWithAllFields)[0][9] == '\0');
  assert(std::get<0>(envpOfExecutableWithAllFields)[1] == NULL);

  const auto executableWithTwoArguments = cu0::Executable{
    .binary = "someBinary",
    .arguments = { "arg1", "arg2", },
    .environment = { { "key", "value", } },
  };

  const auto argvOfExecutableWithTwoArguments =
      cu0::util::argvOf(executableWithTwoArguments);
  assert(std::get<1>(argvOfExecutableWithTwoArguments) == 4);
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0] != NULL);
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][0] == 's');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][1] == 'o');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][2] == 'm');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][3] == 'e');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][4] == 'B');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][5] == 'i');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][6] == 'n');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][7] == 'a');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][8] == 'r');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][9] == 'y');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[0][10] == '\0');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[1] != NULL);
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[1][0] == 'a');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[1][1] == 'r');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[1][2] == 'g');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[1][3] == '1');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[1][4] == '\0');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[2] != NULL);
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[2][0] == 'a');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[2][1] == 'r');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[2][2] == 'g');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[2][3] == '2');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[2][4] == '\0');
  assert(std::get<0>(argvOfExecutableWithTwoArguments)[3] == NULL);

  const auto envpOfExecutableWithTwoArguments =
      cu0::util::envpOf(executableWithTwoArguments);
  assert(std::get<1>(envpOfExecutableWithTwoArguments) == 2);
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0] != NULL);
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][0] == 'k');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][1] == 'e');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][2] == 'y');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][3] == '=');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][4] == 'v');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][5] == 'a');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][6] == 'l');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][7] == 'u');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][8] == 'e');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[0][9] == '\0');
  assert(std::get<0>(envpOfExecutableWithTwoArguments)[1] == NULL);

  const auto executableWithTwoEnvironmentVars = cu0::Executable{
    .binary = "binary1",
    .arguments = {"arg1"},
    .environment = { { "key1", "value1", }, { "key2", "value2", } },
  };

  const auto argvOfExecutableWithTwoEnvironmentVars =
      cu0::util::argvOf(executableWithTwoEnvironmentVars);
  assert(std::get<1>(argvOfExecutableWithTwoEnvironmentVars) == 3);
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0] != NULL);
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0][0] == 'b');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0][1] == 'i');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0][2] == 'n');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0][3] == 'a');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0][4] == 'r');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0][5] == 'y');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0][6] == '1');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[0][7] == '\0');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[1] != NULL);
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[1][0] == 'a');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[1][1] == 'r');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[1][2] == 'g');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[1][3] == '1');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[1][4] == '\0');
  assert(std::get<0>(argvOfExecutableWithTwoEnvironmentVars)[2] == NULL);

  const auto envpOfExecutableWithTwoEnvironmentVars =
      cu0::util::envpOf(executableWithTwoEnvironmentVars);
  assert(std::get<1>(envpOfExecutableWithTwoEnvironmentVars) == 3);
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0] != NULL);
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][0] == 'k');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][1] == 'e');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][2] == 'y');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][3] == '1');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][4] == '=');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][5] == 'v');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][6] == 'a');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][7] == 'l');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][8] == 'u');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][9] == 'e');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][10] == '1');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[0][11] == '\0');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1] != NULL);
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][0] == 'k');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][1] == 'e');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][2] == 'y');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][3] == '2');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][4] == '=');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][5] == 'v');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][6] == 'a');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][7] == 'l');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][8] == 'u');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][9] == 'e');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][10] == '2');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[1][11] == '\0');
  assert(std::get<0>(envpOfExecutableWithTwoEnvironmentVars)[2] == NULL);

  return 0;
}
