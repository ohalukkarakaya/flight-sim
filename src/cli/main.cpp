#include "sim/simulator.h"
#include "core/conflict_sink.h"
#include <iostream>

using namespace fsim;

int main()
{
    auto sink = std::make_shared<ConsoleConflictSink>();
    Simulator sim(1.0, sink); // 1 snlik adımlar

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

    ControlInput inputA;
    ControlInput inputB;

    sim.add_aircraft(a, inputA);
    sim.add_aircraft(b, inputB);

    for (int i = 0; i < 40; ++i) {
        auto conflicts = sim.step();
        std::cout << "t=" << sim.current_time()
                  << "s, conflicts=" << conflicts.size() << "\n";
    }

    return 0;
}