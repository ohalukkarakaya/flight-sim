#include "physics.h"
#include <cmath>

namespace fsim {

// Basit lineer model: hız sabit, konum = konum + v * dt
    AircraftState propagate_state(const AircraftState& current,
                                  const ControlInput& input,
                                  double dtSeconds)
    {
        AircraftState next = current;

        // Yükseklik (z) için climbRate kullan
        next.position.z += input.climbRateMs * dtSeconds;

        // Yakıt tüketimi
        double burned = compute_fuel_burn(current, input, dtSeconds);
        next.fuelKg = std::max(0.0, current.fuelKg - burned);

        // Konum güncelle (x, y sadece mevcut velocity'ye göre)
        next.position.x += current.velocity.x * dtSeconds;
        next.position.y += current.velocity.y * dtSeconds;

        // Şimdilik velocity'yi değiştirmiyoruz, input'tan etkilenmesini sonra ekleyebilirsin
        return next;
    }

    double compute_fuel_burn(const AircraftState& current,
                             const ControlInput& input,
                             double dtSeconds)
    {
        // Çok basit model: throttle ve hızla orantılı tüketim
        const double baseRate = 0.1; // kg/s (uydurma)
        double speed = std::sqrt(current.velocity.x * current.velocity.x +
                                 current.velocity.y * current.velocity.y +
                                 current.velocity.z * current.velocity.z);

        double rate = baseRate * (0.5 + input.throttle) * (1.0 + speed / 200.0);
        return rate * dtSeconds;
    }

} // namespace fsim