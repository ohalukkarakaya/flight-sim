#include "simulator.h"

namespace fsim {

    Simulator::Simulator(double dtSeconds)
            : m_dtSeconds(dtSeconds)
    {
    }

    Simulator::Simulator(double dtSeconds, std::shared_ptr<IConflictSink> sink)
            : m_dtSeconds(dtSeconds),
              m_sink(std::move(sink))
    {
    }

    void Simulator::add_aircraft(const AircraftState& state, const ControlInput& initialInput)
    {
        m_aircraft.push_back(state);
        m_inputs.push_back(initialInput);
        m_autopilots.push_back(std::nullopt);
    }

    void Simulator::add_aircraft_with_autopilot(const AircraftState& state, Autopilot ap)
    {
        m_aircraft.push_back(state);
        m_inputs.push_back(ControlInput{}); // autopilot inputu override edecek
        m_autopilots.push_back(std::move(ap));
    }

    std::vector<Conflict> Simulator::step()
    {
        // Önce autopilotlardan varsa inputları güncelle
        for (std::size_t i = 0; i < m_aircraft.size(); ++i) {
            if (m_autopilots[i].has_value()) {
                m_inputs[i] = m_autopilots[i]->update(m_aircraft[i], m_dtSeconds);
            }
        }

        // Physics
        for (std::size_t i = 0; i < m_aircraft.size(); ++i) {
            m_aircraft[i] = propagate_state(m_aircraft[i], m_inputs[i], m_dtSeconds);
        }

        m_timeSeconds += m_dtSeconds;

        // Conflict detection
        auto conflicts = detect_conflicts(m_aircraft,
                /*horizontalMinMeters=*/500.0,
                /*verticalMinMeters=*/300.0,
                                          m_timeSeconds);

        // Eğer sink enjekte edilmişse ve conflict varsa bildir
        if (m_sink && !conflicts.empty()) {
            m_sink->on_conflicts(conflicts, m_timeSeconds);
        }

        return conflicts;
    }

} // namespace fsim
