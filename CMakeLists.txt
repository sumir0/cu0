cmake_minimum_required(VERSION 3.12)
project(cu0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(cu0_definitions)
if(
    (NOT DEFINED CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT) OR
    (${CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT})
)
  set(cu0_definitions
    "${cu0_definitions} CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT")
  message(STATUS "CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=ON")
else()
  message(STATUS "CU0_DONT_COMPILE_SPECIALIZATION_DECLARATIONS_IN_STRUCT=OFF")
endif()

set(cu0_options)
if(CU0_ENABLE_MORE_WARNINGS)
  set(cu0_options
      $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
      $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
  )
endif()

file(GLOB checks CONFIGURE_DEPENDS checks/*.cc)
foreach(check ${checks})
  string(REGEX REPLACE "\\.[^.]*$" "" check_mid ${check})
  string(REGEX REPLACE ".*checks/" "" check_exe ${check_mid})
  add_executable(${check_exe} ${check})
  target_include_directories(${check_exe} PRIVATE include/)
  target_compile_definitions(${check_exe} PRIVATE ${cu0_definitions})
  target_compile_options(${check_exe} PRIVATE ${cu0_options})
  add_test(NAME ${check_exe} COMMAND ${check_exe})
endforeach()

file(GLOB examples CONFIGURE_DEPENDS examples/*.cc)
foreach(example ${examples})
  string(REGEX REPLACE "\\.[^.]*$" "" example_mid ${example})
  string(REGEX REPLACE ".*examples/" "" example_exe ${example_mid})
  add_executable(${example_exe} ${example})
  target_include_directories(${example_exe} PRIVATE include/)
  target_compile_definitions(${example_exe} PRIVATE ${cu0_definitions})
  target_compile_options(${example_exe} PRIVATE ${cu0_options})
endforeach()

file(GLOB measurements CONFIGURE_DEPENDS measurements/*.cc)
foreach(measurement ${measurements})
  string(REGEX REPLACE "\\.[^.]*$" "" measurement_mid ${measurement})
  string(REGEX REPLACE ".*measurements/" "" measurement_exe ${measurement_mid})
  add_executable(${measurement_exe} ${measurement})
  target_include_directories(${measurement_exe} PRIVATE include/)
  target_compile_definitions(${measurement_exe} PRIVATE ${cu0_definitions})
  target_compile_options(${measurement_exe} PRIVATE ${cu0_options})
endforeach()
