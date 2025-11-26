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

TEST(AutopilotTest, FinishesAfterLastWaypointReached)
{
    AircraftState s;
    s.position = {0.0, 0.0, 3000.0};
    s.velocity = {200.0, 0.0, 0.0};

    Waypoint wp;
    wp.position = {0.0, 0.0, 3000.0};
    wp.targetSpeed = 200.0;

    Autopilot ap({wp},
            /*kpAlt=*/0.01,
            /*kpSpeed=*/0.0,
            /*maxClimbRate=*/10.0,
            /*minThrottle=*/0.0,
            /*maxThrottle=*/1.0);

    // Waypoint'e zaten yakın, ilk update'ten sonra finished olması makul
    ControlInput ci = ap.update(s, 1.0);
    (void)ci;

    EXPECT_TRUE(ap.state().finished);
}

TEST(AutopilotTest, AdvancesThroughMultipleWaypoints)
{
    AircraftState s;
    s.position = {0.0, 0.0, 3000.0};
    s.velocity = {200.0, 0.0, 0.0};

    Waypoint w1;
    w1.position = {0.0, 0.0, 3200.0};
    w1.targetSpeed = 200.0;

    Waypoint w2;
    w2.position = {0.0, 0.0, 3500.0};
    w2.targetSpeed = 200.0;

    std::vector<Waypoint> wps{w1, w2};

    Autopilot ap(wps,
            /*kpAlt=*/0.02,
            /*kpSpeed=*/0.0,
            /*maxClimbRate=*/20.0,
            /*minThrottle=*/0.0,
            /*maxThrottle=*/1.0);

    // 1) Uçağı ilk waypoint'in üstüne "varmış" gibi düşün
    s.position = w1.position;
    ap.update(s, 1.0);

    EXPECT_FALSE(ap.state().finished);
    EXPECT_EQ(ap.state().currentIndex, 1u); // w2'ye geçmeli

    // 2) Uçağı ikinci waypoint'in üstüne getir
    s.position = w2.position;
    ap.update(s, 1.0);

    EXPECT_TRUE(ap.state().finished);
}
