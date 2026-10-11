
#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "rhythm_engine.h"
#include <string>

namespace paul {

bool render_wav(
    const RhythmPattern& pattern,
    const std::string& filename,
    int sample_rate = 44100
);

}

#endif
