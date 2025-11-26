#include <gtest/gtest.h>
#include "core/conflict_detection.h"

using namespace fsim;

TEST(ConflictTest, DetectsHorizontalAndVerticalConflict)
{
    AircraftState a;
    a.id = "A";
    a.position = {0.0, 0.0, 3000.0};

    AircraftState b;
    b.id = "B";
    b.position = {100.0, 0.0, 3200.0}; // yatay 100m, dikey 200m

    double hMin = 500.0;
    double vMin = 300.0;

    EXPECT_TRUE(is_conflict(a, b, hMin, vMin));
}

TEST(ConflictTest, NoConflictWhenFarApart)
{
    AircraftState a;
    a.id = "A";
    a.position = {0.0, 0.0, 3000.0};

    AircraftState b;
    b.id = "B";
    b.position = {10000.0, 0.0, 9000.0};

    EXPECT_FALSE(is_conflict(a, b, 500.0, 300.0));
}

TEST(ConflictTest, DetectConflictsFindsAllPairs)
{
    AircraftState a;
    a.id = "A";
    a.position = {0.0, 0.0, 3000.0};

    AircraftState b;
    b.id = "B";
    b.position = {100.0, 0.0, 3100.0};

    AircraftState c;
    c.id = "C";
    c.position = {10000.0, 0.0, 9000.0};

    std::vector<AircraftState> ac{a, b, c};

    auto conflicts = detect_conflicts(ac,
            /*horizontalMinMeters=*/500.0,
            /*verticalMinMeters=*/300.0,
            /*currentTimeSeconds=*/42.0);

    ASSERT_EQ(conflicts.size(), 1u);
    EXPECT_EQ(conflicts[0].aircraftA, "A");
    EXPECT_EQ(conflicts[0].aircraftB, "B");
    EXPECT_NEAR(conflicts[0].timeSeconds, 42.0, 1e-6);
}

TEST(ConflictTest, AtExactThreshold_IsNotConflict)
{
    AircraftState a;
    a.id = "A";
    a.position = {0.0, 0.0, 3000.0};

    AircraftState b;
    b.id = "B";
    b.position = {500.0, 0.0, 3000.0};  // yatay tam threshold

    double hMin = 500.0;
    double vMin = 300.0;

    EXPECT_FALSE(is_conflict(a, b, hMin, vMin));

    b.position = {0.0, 0.0, 3300.0};    // dikey tam threshold
    EXPECT_FALSE(is_conflict(a, b, hMin, vMin));
}
