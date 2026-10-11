
#include "rhythm_engine.h"

#include <iostream>
#include <stdexcept>

int main() {
    const paul::RhythmPattern pattern = paul::create_rock_pattern(100);

    if (pattern.bpm != 100) {
        std::cerr << "ERROR: BPM incorrecto\n";
        return 1;
    }

    if (pattern.steps_per_bar != 16) {
        std::cerr << "ERROR: divisiones del compas incorrectas\n";
        return 1;
    }

    if (pattern.hits.size() != 12) {
        std::cerr << "ERROR: cantidad de golpes incorrecta\n";
        return 1;
    }

    int kicks = 0;
    int snares = 0;
    int hi_hats = 0;

    for (const auto& hit : pattern.hits) {
        if (hit.step < 0 || hit.step >= 16) {
            std::cerr << "ERROR: golpe fuera del compas\n";
            return 1;
        }

        switch (hit.instrument) {
            case paul::DrumInstrument::Kick:
                ++kicks;
                break;
            case paul::DrumInstrument::Snare:
                ++snares;
                break;
            case paul::DrumInstrument::ClosedHiHat:
                ++hi_hats;
                break;
        }
    }

    if (kicks != 2 || snares != 2 || hi_hats != 8) {
        std::cerr << "ERROR: instrumentos incorrectos\n";
        return 1;
    }

    bool rejected_invalid_bpm = false;

    try {
        paul::create_rock_pattern(300);
    } catch (const std::invalid_argument&) {
        rejected_invalid_bpm = true;
    }

    if (!rejected_invalid_bpm) {
        std::cerr << "ERROR: BPM invalido aceptado\n";
        return 1;
    }

    std::cout << "PRUEBAS RITMICAS SUPERADAS\n";
    std::cout << "BPM: " << pattern.bpm << '\n';
    std::cout << "Golpes: " << pattern.hits.size() << '\n';

    return 0;
}
