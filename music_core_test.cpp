
#include "music_core.h"
#include <cmath>
#include <iostream>

int main() {
    const double la4 = paul::frequency_for_midi(69);
    const int nota_detectada = paul::midi_from_frequency(440.0);

    if (std::abs(la4 - 440.0) > 0.01) {
        std::cout << "ERROR: frecuencia incorrecta\n";
        return 1;
    }

    if (nota_detectada != 69) {
        std::cout << "ERROR: nota incorrecta\n";
        return 1;
    }

    if (paul::midi_from_frequency(0.0) != -1) {
        std::cout << "ERROR: no detecto frecuencia invalida\n";
        return 1;
    }

    std::cout << "PRUEBAS SUPERADAS\n";
    std::cout << "La4 = " << la4 << " Hz\n";
    return 0;
}
