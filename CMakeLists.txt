cmake_minimum_required(VERSION 3.12)
project(cu0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB checks CONFIGURE_DEPENDS checks/*.cc)
foreach(check ${checks})
  string(REGEX REPLACE "\\.[^.]*$" "" check_mid ${check})
  string(REGEX REPLACE ".*checks/" "" check_exe ${check_mid})
  add_executable(${check_exe} ${check})
  target_include_directories(${check_exe} PRIVATE include/)
  add_test(NAME ${check_exe} COMMAND ${check_exe})
endforeach()

file(GLOB examples CONFIGURE_DEPENDS examples/*.cc)
foreach(example ${examples})
  string(REGEX REPLACE "\\.[^.]*$" "" example_mid ${example})
  string(REGEX REPLACE ".*examples/" "" example_exe ${example_mid})
  add_executable(${example_exe} ${example})
  target_include_directories(${example_exe} PRIVATE include/)
endforeach()
