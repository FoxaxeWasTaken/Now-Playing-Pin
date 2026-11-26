# Host build of the portable C++: the renderer and protocol libraries, the simulator and the
# tests.
# The Arduino sketch is not built here. Commands: CLAUDE.md, section "Commands".
cmake_minimum_required(VERSION 3.25)
project(now_playing_pin_host LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NPP_SANITIZE "Build with the address and undefined-behaviour sanitizers" ON)
if(NPP_SANITIZE)
  add_compile_options(-fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

set(NPP_WARNINGS -Wall -Wextra -Werror)

add_library(render render/screen.cpp)
target_include_directories(render PUBLIC render)
target_compile_options(render PRIVATE ${NPP_WARNINGS})

add_library(protocol protocol/protocol.cpp)
target_include_directories(protocol PUBLIC protocol)
target_compile_options(protocol PRIVATE ${NPP_WARNINGS})

add_executable(sim sim/main.cpp)
target_link_libraries(sim PRIVATE render)
target_compile_options(sim PRIVATE ${NPP_WARNINGS})

include(FetchContent)
FetchContent_Declare(
  doctest
  URL https://github.com/doctest/doctest/archive/refs/tags/v2.5.3.tar.gz
  URL_HASH SHA256=174ebc4e769928959614789c5b4e9c3d0a0f81a62bb608756b127bfebfb21331
  SYSTEM
)
FetchContent_MakeAvailable(doctest)

enable_testing()
add_executable(render_tests tests/screen_test.cpp)
target_link_libraries(render_tests PRIVATE render doctest::doctest_with_main)
target_compile_options(render_tests PRIVATE ${NPP_WARNINGS})
add_test(NAME render_tests COMMAND render_tests)

add_executable(protocol_tests tests/protocol_test.cpp)
target_link_libraries(protocol_tests PRIVATE protocol doctest::doctest_with_main)
target_compile_options(protocol_tests PRIVATE ${NPP_WARNINGS})
target_compile_definitions(protocol_tests PRIVATE NPP_VECTORS_DIR="${PROJECT_SOURCE_DIR}/../tools/vectors")
add_test(NAME protocol_tests COMMAND protocol_tests)
