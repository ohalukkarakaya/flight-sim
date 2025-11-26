#include <gtest/gtest.h>
#include "sim/simulator.h"
#include "core/autopilot.h"

using namespace fsim;

// 1) Head-on çarpışma senaryosu (integration / scenario test)
TEST(ScenarioTest, HeadOnConflictAppearsWithinTime)
{
    Simulator sim(1.0); // 1s step

    AircraftState a;
    a.id = "A";
    a.position = {0.0, 0.0, 3000.0};
    a.velocity = {250.0, 0.0, 0.0};
    a.fuelKg = 500.0;

    AircraftState b;
    b.id = "B";
    b.position = {5000.0, 0.0, 3000.0};
    b.velocity = {-250.0, 0.0, 0.0};
    b.fuelKg = 500.0;

    ControlInput ia;
    ControlInput ib;

    sim.add_aircraft(a, ia);
    sim.add_aircraft(b, ib);

    bool conflictSeen = false;
    for (int i = 0; i < 40; ++i) {
        auto conflicts = sim.step();
        if (!conflicts.empty()) {
            conflictSeen = true;
            break;
        }
    }

    EXPECT_TRUE(conflictSeen);
}

// 2) Autopilot + simulator entegrasyon senaryosu
TEST(ScenarioTest, AutopilotClimbsToWaypointAltitude)
{
    Simulator sim(1.0); // 1 saniyelik step

    AircraftState s;
    s.id = "A";
    s.position = {0.0, 0.0, 3000.0};     // başlangıç irtifası
    s.velocity = {200.0, 0.0, 0.0};      // yatay hız
    s.fuelKg = 1000.0;

    Waypoint wp;
    wp.position = {5000.0, 0.0, 5000.0}; // 2000m yukarı + 5km ileride
    wp.targetSpeed = 200.0;

    Autopilot ap({wp},
            /*kpAlt=*/0.02,   // biraz daha agresif yükselme
            /*kpSpeed=*/0.0,
            /*maxClimbRate=*/20.0,
            /*minThrottle=*/0.0,
            /*maxThrottle=*/1.0);

    sim.add_aircraft_with_autopilot(s, std::move(ap));

    // 200 saniye simüle et
    for (int i = 0; i < 200; ++i) {
        sim.step();
    }

    const auto& finalState = sim.aircraft().front();

    // Davranış testi:
    // - Başta 3000m'deydik, en az 4000m'nin üstüne çıkmış olmalı
    // - Ama saçma sapan 10km'lere de gitmemeli
    EXPECT_GT(finalState.position.z, 4000.0);
    EXPECT_LT(finalState.position.z, 7000.0);
}

TEST(ScenarioTest, ParallelFlight_NoConflictOccurs)
{
    Simulator sim(1.0);

    AircraftState a;
    a.id = "A";
    a.position = {0.0, 0.0, 3000.0};
    a.velocity = {200.0, 0.0, 0.0};
    a.fuelKg = 500.0;

    AircraftState b;
    b.id = "B";
    b.position = {0.0, 5000.0, 3000.0}; // 5km yanda, aynı yönde
    b.velocity = {200.0, 0.0, 0.0};
    b.fuelKg = 500.0;

    ControlInput ia;
    ControlInput ib;

    sim.add_aircraft(a, ia);
    sim.add_aircraft(b, ib);

    bool conflictSeen = false;
    for (int i = 0; i < 120; ++i) { // 2 dakika
        auto conflicts = sim.step();
        if (!conflicts.empty()) {
            conflictSeen = true;
            break;
        }
    }

    EXPECT_FALSE(conflictSeen);
}

TEST(ScenarioTest, SameInitialConditionsProduceSameResult)
{
    Simulator sim1(1.0);
    Simulator sim2(1.0);

    AircraftState a;
    a.id = "A";
    a.position = {0.0, 0.0, 3000.0};
    a.velocity = {250.0, 0.0, 0.0};
    a.fuelKg = 500.0;

    AircraftState b;
    b.id = "B";
    b.position = {5000.0, 0.0, 3000.0};
    b.velocity = {-250.0, 0.0, 0.0};
    b.fuelKg = 500.0;

    ControlInput ia;
    ControlInput ib;

    sim1.add_aircraft(a, ia);
    sim1.add_aircraft(b, ib);

    sim2.add_aircraft(a, ia);
    sim2.add_aircraft(b, ib);

    for (int i = 0; i < 40; ++i) {
        sim1.step();
        sim2.step();
    }

    const auto& a1 = sim1.aircraft()[0];
    const auto& b1 = sim1.aircraft()[1];
    const auto& a2 = sim2.aircraft()[0];
    const auto& b2 = sim2.aircraft()[1];

    EXPECT_NEAR(a1.position.x, a2.position.x, 1e-9);
    EXPECT_NEAR(a1.position.y, a2.position.y, 1e-9);
    EXPECT_NEAR(a1.position.z, a2.position.z, 1e-9);

    EXPECT_NEAR(b1.position.x, b2.position.x, 1e-9);
    EXPECT_NEAR(b1.position.y, b2.position.y, 1e-9);
    EXPECT_NEAR(b1.position.z, b2.position.z, 1e-9);
}

