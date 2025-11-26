#pragma once

#include <string>

namespace fsim {

    struct Vec3 {
        double x{0.0};
        double y{0.0};
        double z{0.0}; // metre cinsinden irtifa için de kullanabiliriz
    };

    struct AircraftState {
        std::string id;
        Vec3 position;     // metre
        Vec3 velocity;     // m/s
        double headingDeg{0.0};  // 0–360, sadece yorum amaçlı
        double fuelKg{0.0};      // yakıt miktarı
    };

    struct ControlInput {
        // Şimdilik basit tutuyoruz, sonra genişletebilirsin
        double throttle{1.0};     // 0–1
        double climbRateMs{0.0};  // dikey hız komutu (m/s)
    };

} // namespace fsim
