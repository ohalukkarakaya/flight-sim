#include <gtest/gtest.h>
#include "sim/simulator.h"
#include "core/autopilot.h"

using namespace fsim;

TEST(ScenarioTest, AutopilotClimbsToWaypointAltitude)
{
    Simulator sim(1.0); // 1 saniyelik step

    AircraftState s;
    s.id = "A";
    s.position = {0.0, 0.0, 3000.0};
    s.velocity = {200.0, 0.0, 0.0};
    s.fuelKg = 1000.0;

    Waypoint wp;
    wp.position = {5000.0, 0.0, 5000.0}; // 2000m yukarı + 5km ileride
    wp.targetSpeed = 200.0;

    Autopilot ap({wp},
            /*kpAlt=*/0.01,
            /*kpSpeed=*/0.0,   // hız kontrolü şimdilik önemli değil
            /*maxClimbRate=*/20.0,
            /*minThrottle=*/0.0,
            /*maxThrottle=*/1.0);

    sim.add_aircraft_with_autopilot(s, std::move(ap));

    // 200 saniye simüle et
    for (int i = 0; i < 200; ++i) {
        sim.step();
    }

    const auto& finalState = sim.aircraft().front();
    // Hedef irtifaya yakın olmasını bekliyoruz
    EXPECT_NEAR(finalState.position.z, 5000.0, 200.0); // ±200m tolerans
}
