include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# Sadece testler için derlensin
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(flight_sim_tests
        test_physics.cpp
        test_conflict.cpp
        test_scenarios.cpp
        test_autopilot.cpp
)

target_link_libraries(flight_sim_tests
        PRIVATE
        flight_sim_core
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(flight_sim_tests)