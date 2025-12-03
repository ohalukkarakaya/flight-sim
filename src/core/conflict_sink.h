#pragma once

#include <vector>
#include <memory>
#include "core/conflict_detection.h"

namespace fsim {

// Çarpışma bilgilerini "bir yere" göndermek için arayüz.
// Production'da console'a yazabilir, dosyaya loglayabilir,
// testte ise sahte bir sınıf ile gözlemlenir.
    struct IConflictSink {
        virtual ~IConflictSink() = default;

        virtual void on_conflicts(const std::vector<Conflict>& conflicts,
                                  double currentTimeSeconds) = 0;
    };

// prod için: conflict'leri std::cout'a yazar.
    class ConsoleConflictSink : public IConflictSink {
    public:
        void on_conflicts(const std::vector<Conflict>& conflicts,
                          double currentTimeSeconds) override;
    };

} // namespace fsim