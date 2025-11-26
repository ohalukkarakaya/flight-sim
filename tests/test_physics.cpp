#include <gtest/gtest.h>
#include "core/physics.h"

using namespace fsim;

TEST(PhysicsTest, PropagateStateMovesLinearly)
{
    AircraftState s;
    s.position = {0.0, 0.0, 3000.0};
    s.velocity = {100.0, 0.0, 0.0}; // 100 m/s
    s.fuelKg = 100.0;

    ControlInput input;
    input.climbRateMs = 0.0;
    input.throttle = 0.5;

    double dt = 10.0; // 10 saniye
    AircraftState next = propagate_state(s, input, dt);

    // x = v * t
    EXPECT_NEAR(next.position.x, 100.0 * dt, 1e-6);
    EXPECT_NEAR(next.position.y, 0.0, 1e-6);
    EXPECT_NEAR(next.position.z, 3000.0, 1e-6);

    // Yakıtın azalmış olması gerekiyor
    EXPECT_LT(next.fuelKg, s.fuelKg);
}

TEST(PhysicsTest, FuelBurnIncreasesWithThrottle)
{
    AircraftState s;
    s.position = {0.0, 0.0, 0.0};
    s.velocity = {200.0, 0.0, 0.0};
    s.fuelKg = 1000.0;

    double dt = 5.0;

    ControlInput low;
    low.throttle = 0.2;
    low.climbRateMs = 0.0;

    ControlInput high;
    high.throttle = 1.0;
    high.climbRateMs = 0.0;

    double burnLow  = compute_fuel_burn(s, low, dt);
    double burnHigh = compute_fuel_burn(s, high, dt);

    EXPECT_GT(burnHigh, burnLow);
}

TEST(PhysicsTest, ZeroDtDoesNotChangeState)
{
    AircraftState s;
    s.position = {1000.0, 2000.0, 5000.0};
    s.velocity = {150.0, -20.0, 0.0};
    s.fuelKg = 300.0;

    ControlInput input;
    input.climbRateMs = 5.0;
    input.throttle = 1.0;

    double dt = 0.0;
    AircraftState next = propagate_state(s, input, dt);

    EXPECT_NEAR(next.position.x, s.position.x, 1e-9);
    EXPECT_NEAR(next.position.y, s.position.y, 1e-9);
    EXPECT_NEAR(next.position.z, s.position.z, 1e-9);
    EXPECT_NEAR(next.fuelKg, s.fuelKg, 1e-9);
}

TEST(PhysicsTest, FuelDoesNotGoNegative)
{
    AircraftState s;
    s.position = {0.0, 0.0, 1000.0};
    s.velocity = {200.0, 0.0, 0.0};
    s.fuelKg = 1.0; // çok az yakıt

    ControlInput input;
    input.throttle = 1.0;
    input.climbRateMs = 0.0;

    double dt = 100.0; // saçma büyük dt
    AircraftState next = propagate_state(s, input, dt);

    EXPECT_GE(next.fuelKg, 0.0);
}