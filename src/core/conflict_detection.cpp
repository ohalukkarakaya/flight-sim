#include "conflict_detection.h"
#include <cmath>

namespace fsim {

    bool is_conflict(const AircraftState& a,
                     const AircraftState& b,
                     double horizontalMinMeters,
                     double verticalMinMeters)
    {
        double dx = a.position.x - b.position.x;
        double dy = a.position.y - b.position.y;
        double dz = a.position.z - b.position.z;

        double horizontalDist = std::sqrt(dx*dx + dy*dy);
        double verticalDist   = std::fabs(dz);

        return (horizontalDist < horizontalMinMeters) &&
               (verticalDist < verticalMinMeters);
    }

    std::vector<Conflict> detect_conflicts(const std::vector<AircraftState>& aircraft,
                                           double horizontalMinMeters,
                                           double verticalMinMeters,
                                           double currentTimeSeconds)
    {
        std::vector<Conflict> result;
        for (std::size_t i = 0; i < aircraft.size(); ++i) {
            for (std::size_t j = i + 1; j < aircraft.size(); ++j) {
                if (is_conflict(aircraft[i], aircraft[j],
                                horizontalMinMeters, verticalMinMeters)) {
                    result.push_back(Conflict{
                            aircraft[i].id,
                            aircraft[j].id,
                            currentTimeSeconds
                    });
                }
            }
        }
        return result;
    }

} // namespace fsim