#include "autopilot.h"
#include <cmath>
#include <algorithm>

namespace fsim {

    Autopilot::Autopilot(std::vector<Waypoint> waypoints,
                         double kpAlt,
                         double kpSpeed,
                         double maxClimbRate,
                         double minThrottle,
                         double maxThrottle)
            : m_waypoints(std::move(waypoints)),
              m_kpAlt(kpAlt),
              m_kpSpeed(kpSpeed),
              m_maxClimbRate(maxClimbRate),
              m_minThrottle(minThrottle),
              m_maxThrottle(maxThrottle)
    {
        if (m_waypoints.empty()) {
            m_state.finished = true;
        }
    }

    double Autopilot::current_target_speed() const {
        if (m_state.finished || m_waypoints.empty()) return 0.0;
        return m_waypoints[m_state.currentIndex].targetSpeed;
    }

    double Autopilot::current_target_altitude() const {
        if (m_state.finished || m_waypoints.empty()) return 0.0;
        return m_waypoints[m_state.currentIndex].position.z;
    }

    ControlInput Autopilot::update(const AircraftState& state, double /*dtSeconds*/)
    {
        ControlInput ci{};

        if (m_state.finished || m_waypoints.empty()) {
            ci.throttle = 0.0;
            ci.climbRateMs = 0.0;
            return ci;
        }

        const auto& wp = m_waypoints[m_state.currentIndex];

        // hedefe yatay yakınsa bir sonraki waypoint'e geç
        double dx = state.position.x - wp.position.x;
        double dy = state.position.y - wp.position.y;
        double horizDist = std::sqrt(dx*dx + dy*dy);
        if (horizDist < 100.0 && std::fabs(state.position.z - wp.position.z) < 50.0) {
            if (m_state.currentIndex + 1 < m_waypoints.size()) {
                m_state.currentIndex++;
            } else {
                m_state.finished = true;
            }
        }

        double targetAlt = current_target_altitude();
        double targetSpeed = current_target_speed();

        double altError = targetAlt - state.position.z;

        ci.climbRateMs = m_kpAlt * altError;
        ci.climbRateMs = std::clamp(ci.climbRateMs, -m_maxClimbRate, m_maxClimbRate);

        double vx = state.velocity.x;
        double vy = state.velocity.y;
        double currentSpeed = std::sqrt(vx*vx + vy*vy);
        double speedError = targetSpeed - currentSpeed;

        double rawThrottle = 0.5 + m_kpSpeed * speedError; // 0.5 baz, üstüne düzeltme
        ci.throttle = std::clamp(rawThrottle, m_minThrottle, m_maxThrottle);

        return ci;
    }

} // namespace fsim