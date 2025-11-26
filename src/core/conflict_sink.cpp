#include "core/conflict_sink.h"
#include <iostream>

namespace fsim {

    void ConsoleConflictSink::on_conflicts(const std::vector<Conflict>& conflicts,
                                           double currentTimeSeconds)
    {
        for (const auto& c : conflicts) {
            std::cout << "[ALERT t=" << currentTimeSeconds << "s] "
                      << c.aircraftA << " vs " << c.aircraftB << "\n";
        }
    }

} // namespace fsim