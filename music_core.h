
#ifndef MUSIC_CORE_H
#define MUSIC_CORE_H

namespace paul {

double frequency_for_midi(int midi_note);
int midi_from_frequency(double frequency_hz);

}

#endif
