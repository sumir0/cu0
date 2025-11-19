cmake_minimum_required(VERSION 3.12)
project(cu0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(definitions)
if(
    (NOT DEFINED CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT) OR
    (${CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT})
)
  set(definitions
    "${definitions} CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT")
  message(STATUS "CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=ON")
else()
  message(STATUS "CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=OFF")
endif()

file(GLOB checks CONFIGURE_DEPENDS checks/*.cc)
foreach(check ${checks})
  string(REGEX REPLACE "\\.[^.]*$" "" check_mid ${check})
  string(REGEX REPLACE ".*checks/" "" check_exe ${check_mid})
  add_executable(${check_exe} ${check})
  target_include_directories(${check_exe} PRIVATE include/)
  target_compile_definitions(${check_exe} PRIVATE ${definitions})
  add_test(NAME ${check_exe} COMMAND ${check_exe})
endforeach()

file(GLOB examples CONFIGURE_DEPENDS examples/*.cc)
foreach(example ${examples})
  string(REGEX REPLACE "\\.[^.]*$" "" example_mid ${example})
  string(REGEX REPLACE ".*examples/" "" example_exe ${example_mid})
  add_executable(${example_exe} ${example})
  target_include_directories(${example_exe} PRIVATE include/)
  target_compile_definitions(${example_exe} PRIVATE ${definitions})
endforeach()

file(GLOB measurements CONFIGURE_DEPENDS measurements/*.cc)
foreach(measurement ${measurements})
  string(REGEX REPLACE "\\.[^.]*$" "" measurement_mid ${measurement})
  string(REGEX REPLACE ".*measurements/" "" measurement_exe ${measurement_mid})
  add_executable(${measurement_exe} ${measurement})
  target_include_directories(${measurement_exe} PRIVATE include/)
  target_compile_definitions(${measurement_exe} PRIVATE ${definitions})
endforeach()
