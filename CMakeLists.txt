cmake_minimum_required(VERSION 3.7)

project(progetto_sistemi_operativi)

# Options
add_compile_options(-Wall -Wextra -pedantic -fdiagnostics-color) 

set( CLIENT_SRC
  src/defines.c
  src/client_lib.c
  src/position.c
  src/fifo.c
  src/message.c
  src/err_exit.c
)
set( SERVER_SRC
  src/server_lib.c
  src/err_exit.c
  src/defines.c
  src/shared_memory.c
  src/semaphore.c
  src/fifo.c
  src/position.c
  src/message.c
  src/files.c
)

include_directories(src/inc)
add_executable(client src/client.c ${CLIENT_SRC})
add_executable(server src/server.c ${SERVER_SRC})

# Testing
enable_testing()
include(GoogleTest)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

file(GLOB TESTS_SRC "src/test/*.cpp")

add_executable(tests ${TESTS_SRC} ${CLIENT_SRC} ${SERVER_SRC})
target_link_libraries(tests gtest_main)
gtest_add_tests(TARGET tests TEST_LIST test_all)
set_tests_properties(${test_all} PROPERTIES TIMEOUT 10)
