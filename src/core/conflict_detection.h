#pragma once

#include "aircraft_state.h"
#include <vector>

namespace fsim {

    struct Conflict {
        std::string aircraftA;
        std::string aircraftB;
        double timeSeconds; // tespit zamanı
    };

    bool is_conflict(const AircraftState& a,
                     const AircraftState& b,
                     double horizontalMinMeters,
                     double verticalMinMeters);

// Bir sürü uçak içinden tüm conflict'leri bul
    std::vector<Conflict> detect_conflicts(const std::vector<AircraftState>& aircraft,
                                           double horizontalMinMeters,
                                           double verticalMinMeters,
                                           double currentTimeSeconds);

} // namespace fsim