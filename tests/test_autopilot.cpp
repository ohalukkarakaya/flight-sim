#include <gtest/gtest.h>
#include "core/autopilot.h"

using namespace fsim;

TEST(AutopilotTest, BelowTargetAltitude_CommandsPositiveClimbRate)
{
    AircraftState s;
    s.position = {0.0, 0.0, 3000.0};
    s.velocity = {200.0, 0.0, 0.0};

    Waypoint wp;
    wp.position = {1000.0, 0.0, 5000.0}; // 2000m yukarıda
    wp.targetSpeed = 220.0;

    Autopilot ap({wp},
            /*kpAlt=*/0.01,
            /*kpSpeed=*/0.005,
            /*maxClimbRate=*/20.0,
            /*minThrottle=*/0.0,
            /*maxThrottle=*/1.0);

    ControlInput ci = ap.update(s, 1.0);

    EXPECT_GT(ci.climbRateMs, 0.0);        // aşağıdaysak yukarı tırmansın
    EXPECT_GE(ci.throttle, 0.0);
    EXPECT_LE(ci.throttle, 1.0);
}

TEST(AutopilotTest, AboveTargetAltitude_CommandsNegativeClimbRate)
{
    AircraftState s;
    s.position = {0.0, 0.0, 6000.0}; // waypoint'in üstünde
    s.velocity = {200.0, 0.0, 0.0};

    Waypoint wp;
    wp.position = {0.0, 0.0, 4000.0};
    wp.targetSpeed = 200.0;

    Autopilot ap({wp},
            /*kpAlt=*/0.01,
            /*kpSpeed=*/0.005,
            /*maxClimbRate=*/20.0,
            /*minThrottle=*/0.0,
            /*maxThrottle=*/1.0);

    ControlInput ci = ap.update(s, 1.0);

    EXPECT_LT(ci.climbRateMs, 0.0); // yukarıdaysak aşağı insin
}