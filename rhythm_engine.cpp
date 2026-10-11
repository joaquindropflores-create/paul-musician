
#include "rhythm_engine.h"

#include <stdexcept>

namespace paul {

RhythmPattern create_rock_pattern(int bpm) {
    if (bpm < 40 || bpm > 240) {
        throw std::invalid_argument("BPM debe estar entre 40 y 240");
    }

    RhythmPattern pattern;
    pattern.bpm = bpm;
    pattern.steps_per_bar = 16;

    // Bombo en los tiempos 1 y 3.
    pattern.hits.push_back({0, DrumInstrument::Kick, 0.95f});
    pattern.hits.push_back({8, DrumInstrument::Kick, 0.90f});

    // Caja en los tiempos 2 y 4.
    pattern.hits.push_back({4, DrumInstrument::Snare, 0.90f});
    pattern.hits.push_back({12, DrumInstrument::Snare, 0.95f});

    // Platillo cerrado en cada pulso de corchea.
    for (int step = 0; step < 16; step += 2) {
        pattern.hits.push_back({
            step,
            DrumInstrument::ClosedHiHat,
            0.60f
        });
    }

    return pattern;
}

std::string instrument_name(DrumInstrument instrument) {
    switch (instrument) {
        case DrumInstrument::Kick:
            return "Bombo";
        case DrumInstrument::Snare:
            return "Caja";
        case DrumInstrument::ClosedHiHat:
            return "Platillo cerrado";
    }

    return "Desconocido";
}

}
