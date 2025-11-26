#pragma once

#include "core/aircraft_state.h"
#include "core/conflict_detection.h"
#include "core/physics.h"
#include "core/autopilot.h"
#include <vector>
#include <optional>

namespace fsim {

    class Simulator {
    public:
        explicit Simulator(double dtSeconds);

        // Manuel kontrol
        void add_aircraft(const AircraftState& state, const ControlInput& initialInput);

        // Autopilot ile kontrol edilen uçak
        void add_aircraft_with_autopilot(const AircraftState& state, Autopilot ap);

        std::vector<Conflict> step();

        double current_time() const { return m_timeSeconds; }
        const std::vector<AircraftState>& aircraft() const { return m_aircraft; }

    private:
        double m_dtSeconds;
        double m_timeSeconds{0.0};
        std::vector<AircraftState> m_aircraft;
        std::vector<ControlInput>  m_inputs;
        // Autopilot varsa burada tutulur, yoksa std::nullopt:
        std::vector<std::optional<Autopilot>> m_autopilots;
    };

} // namespace fsim