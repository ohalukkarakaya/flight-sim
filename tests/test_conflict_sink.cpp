#include <gtest/gtest.h>
#include "core/conflict_sink.h"
#include "sim/simulator.h"

using namespace fsim;

// Test double: gerçek IO yapmayan, sadece çağrıları kaydeden "fake" sink
struct FakeConflictSink : public IConflictSink {
    std::vector<std::vector<Conflict>> calls;
    std::vector<double> times;

    void on_conflicts(const std::vector<Conflict>& conflicts,
                      double currentTimeSeconds) override {
        calls.push_back(conflicts);
        times.push_back(currentTimeSeconds);
    }
};

TEST(ConflictSinkTest, NoConflicts_NoCallsToSink)
{
    auto fakeSink = std::make_shared<FakeConflictSink>();
    Simulator sim(1.0, fakeSink);

    AircraftState a;
    a.id = "A";
    a.position = {0.0, 0.0, 3000.0};
    a.velocity = {200.0, 0.0, 0.0};
    a.fuelKg = 500.0;

    AircraftState b;
    b.id = "B";
    b.position = {0.0, 5000.0, 3000.0};  // 5km yanda, çatışma yok
    b.velocity = {200.0, 0.0, 0.0};
    b.fuelKg = 500.0;

    ControlInput ia;
    ControlInput ib;

    sim.add_aircraft(a, ia);
    sim.add_aircraft(b, ib);

    for (int i = 0; i < 60; ++i) {
        sim.step();
    }

    EXPECT_TRUE(fakeSink->calls.empty());
    EXPECT_TRUE(fakeSink->times.empty());
}

TEST(ConflictSinkTest, HeadOnScenario_TriggersSinkAtLeastOnce)
{
    auto fakeSink = std::make_shared<FakeConflictSink>();
    Simulator sim(1.0, fakeSink);

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

    for (int i = 0; i < 40; ++i) {
        sim.step();
    }

    // En az bir kere conflict sink'e bildirilmiş olmalı
    ASSERT_FALSE(fakeSink->calls.empty());

    bool foundNonEmpty = false;
    for (const auto& v : fakeSink->calls) {
        if (!v.empty()) {
            foundNonEmpty = true;
            break;
        }
    }
    EXPECT_TRUE(foundNonEmpty);

    // Zaman bilgisi de kaydedilmiş olmalı
    ASSERT_EQ(fakeSink->calls.size(), fakeSink->times.size());
}