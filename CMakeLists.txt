cmake_minimum_required(VERSION 3.16)
project(flight_sim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(flight_sim_core
        src/core/aircraft_state.h
        src/core/physics.h
        src/core/physics.cpp
        src/core/conflict_detection.h
        src/core/conflict_detection.cpp
        src/core/autopilot.h
        src/core/autopilot.cpp
        src/core/conflict_sink.h
        src/core/conflict_sink.cpp
        src/sim/simulator.h
        src/sim/simulator.cpp
)

target_include_directories(flight_sim_core PUBLIC src)

add_executable(flight_sim_cli
        src/cli/main.cpp
)
target_link_libraries(flight_sim_cli PRIVATE flight_sim_core)

# TESTLER
enable_testing()
add_subdirectory(tests)