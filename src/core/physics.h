#pragma once

#include "aircraft_state.h"

namespace fsim {

// Tek bir uçağın dt süresindeki yeni durumunu hesaplar.
// PURE FUNCTION: state'i değiştirmez, yenisini döner.
    AircraftState propagate_state(const AircraftState& current,
                                  const ControlInput& input,
                                  double dtSeconds);

// Basit yakıt tüketimi modeli (opsiyonel, test için güzel).
    double compute_fuel_burn(const AircraftState& current,
                             const ControlInput& input,
                             double dtSeconds);

} // namespace fsim