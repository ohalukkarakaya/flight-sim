#pragma once

#include "core/aircraft_state.h"
#include "core/conflict_detection.h"
#include "core/physics.h"
#include "core/autopilot.h"
#include "core/conflict_sink.h"
#include <vector>
#include <optional>
#include <memory>

namespace fsim {

    class Simulator {
    public:
        // Eski constructor (sink olmadan da kullanılabilir)
        explicit Simulator(double dtSeconds);

        // DI ile conflict sink enjekte edilen constructor
        Simulator(double dtSeconds, std::shared_ptr<IConflictSink> sink);

        // Sonradan da set edebilmek için
        void set_conflict_sink(std::shared_ptr<IConflictSink> sink) { m_sink = std::move(sink); }

        // Manuel kontrol
        void add_aircraft(const AircraftState& state, const ControlInput& initialInput);

        // Autopilot ile kontrol edilen uçak
        void add_aircraft_with_autopilot(const AircraftState& state, Autopilot ap);

        // Bir simülasyon adımı ilerle
        std::vector<Conflict> step();

        double current_time() const { return m_timeSeconds; }
        const std::vector<AircraftState>& aircraft() const { return m_aircraft; }

    private:
        double m_dtSeconds;
        double m_timeSeconds{0.0};
        std::vector<AircraftState> m_aircraft;
        std::vector<ControlInput>  m_inputs;
        std::vector<std::optional<Autopilot>> m_autopilots;

        std::shared_ptr<IConflictSink> m_sink;
    };

} // namespace fsim
