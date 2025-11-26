#include <gtest/gtest.h>
#include "core/physics.h"

using namespace fsim;

TEST(PhysicsTest, PropagateStateMovesLinearly)
{
    AircraftState s;
    s.position = {0.0, 0.0, 3000.0};
    s.velocity = {100.0, 0.0, 0.0};
    s.fuelKg = 100.0;

    ControlInput input;
    input.climbRateMs = 0.0;
    input.throttle = 0.5;

    double dt = 10.0; // 10 saniye
    AircraftState next = propagate_state(s, input, dt);

    EXPECT_NEAR(next.position.x, 100.0 * dt, 1e-6);
    EXPECT_NEAR(next.position.y, 0.0, 1e-6);
    EXPECT_NEAR(next.position.z, 3000.0, 1e-6);
    EXPECT_LT(next.fuelKg, s.fuelKg); // yakıt azalmış olmalı
}