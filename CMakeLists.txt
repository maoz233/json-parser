cmake_minimum_required(VERSION 3.24.0)

project(jpp VERSION 1.0)

message(STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
message(STATUS "PROJECR_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler options
if(MSVC)
  # warning level 4 and all warnings as errors
  add_compile_options(/W4 /WX)
else()
  # lots of warnings and all warnings as errors
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(LIB_NAME jpp_lib)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
list(APPEND EXTRA_LIBS ${LIB_NAME})
list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  jpp_test 
  ${PROJECT_SOURCE_DIR}/test/main.test.cc 
  ${PROJECT_SOURCE_DIR}/test/json.test.cc
)
target_link_libraries(
  jpp_test 
  GTest::gtest_main
  ${EXTRA_LIBS}
)
target_include_directories(jpp_test PUBLIC ${PROJECT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(jpp_test)
