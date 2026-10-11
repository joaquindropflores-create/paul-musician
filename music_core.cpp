
#include "music_core.h"
#include <cmath>

namespace paul {

double frequency_for_midi(int midi_note) {
    return 440.0 * std::pow(2.0, (midi_note - 69) / 12.0);
}

int midi_from_frequency(double frequency_hz) {
    if (frequency_hz <= 0.0) {
        return -1;
    }

    return static_cast<int>(
        std::round(69.0 + 12.0 * std::log2(frequency_hz / 440.0))
    );
}

}
