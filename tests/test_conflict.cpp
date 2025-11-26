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
    b.position = {100.0, 0.0, 3200.0};

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
