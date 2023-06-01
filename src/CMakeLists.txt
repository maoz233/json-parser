set(LIB_NAME jpp_lib)

add_library(${LIB_NAME} STATIC ${PROJECT_SOURCE_DIR}/src/json.cc)

target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Compiler options
if(MSVC)
  # warning level 4 and all warnings as errors
  target_compile_options(${LIB_NAME} PRIVATE /W4 /WX)
else()
  # lots of warnings and all warnings as errors
  target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

