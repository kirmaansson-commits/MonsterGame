cmake_minimum_required(VERSION 3.16)
project(MonsterGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(MonsterGame ${SOURCES})

target_include_directories(MonsterGame PUBLIC include)

