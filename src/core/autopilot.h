#pragma once

#include "aircraft_state.h"
#include <vector>

namespace fsim {

    struct Waypoint {
        Vec3 position;      // hedef nokta (x,y,z)
        double targetSpeed; // m/s
    };

    struct AutopilotState {
        std::size_t currentIndex{0};
        bool finished{false};
    };

    class Autopilot {
    public:
        Autopilot(std::vector<Waypoint> waypoints,
                  double kpAlt,
                  double kpSpeed,
                  double maxClimbRate,
                  double minThrottle,
                  double maxThrottle);

        // Mevcut uçak durumuna göre bir ControlInput üretir.
        // PURE-ish: dış world'e dokunmuyor, sadece internal state tutuyor.
        ControlInput update(const AircraftState& state, double dtSeconds);

        const AutopilotState& state() const { return m_state; }

    private:
        std::vector<Waypoint> m_waypoints;
        AutopilotState m_state;

        double m_kpAlt;
        double m_kpSpeed;
        double m_maxClimbRate;
        double m_minThrottle;
        double m_maxThrottle;

        double current_target_speed() const;
        double current_target_altitude() const;
    };

} // namespace fsim