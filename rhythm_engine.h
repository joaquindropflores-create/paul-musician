
#ifndef RHYTHM_ENGINE_H
#define RHYTHM_ENGINE_H

#include <string>
#include <vector>

namespace paul {

enum class DrumInstrument {
    Kick,
    Snare,
    ClosedHiHat
};

struct DrumHit {
    int step;
    DrumInstrument instrument;
    float velocity;
};

struct RhythmPattern {
    int bpm;
    int steps_per_bar;
    std::vector<DrumHit> hits;
};

RhythmPattern create_rock_pattern(int bpm = 100);

std::string instrument_name(DrumInstrument instrument);

}

#endif
