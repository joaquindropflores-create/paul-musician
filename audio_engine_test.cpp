
#include "audio_engine.h"

#include <fstream>
#include <iostream>
#include <string>

int main() {
    const std::string filename = "paul_test.wav";

    const paul::RhythmPattern pattern =
        paul::create_rock_pattern(100);

    if (!paul::render_wav(pattern, filename)) {
        std::cerr << "ERROR: no se pudo generar el WAV\n";
        return 1;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "ERROR: no se encontro el archivo\n";
        return 1;
    }

    char header[12] = {};
    file.read(header, sizeof(header));

    if (!file) {
    std::cerr << "ERROR: no se pudieron leer 12 bytes\n";
    return 1;
}

std::cerr << "Diagnostico WAV: ";
for (int i = 0; i < 12; ++i) {
    const unsigned char c =
        static_cast<unsigned char>(header[i]);

    if (c >= 32 && c <= 126) {
        std::cerr << static_cast<char>(c);
    } else {
        std::cerr << "[" << static_cast<int>(c) << "]";
    }
}
std::cerr << '\n';

if (std::string(header, 4) != "RIFF" ||
    std::string(header + 8, 4) != "WAVE") {
    std::cerr << "ERROR: cabecera WAV incorrecta\n";
    return 1;
}

    file.seekg(0, std::ios::end);

    if (file.tellg() <= 44) {
        std::cerr << "ERROR: archivo de audio vacio\n";
        return 1;
    }

    std::cout << "AUDIO GENERADO CORRECTAMENTE\n";
    std::cout << "Formato: WAV mono, 16 bits\n";
    std::cout << "Frecuencia: 44100 Hz\n";
    std::cout << "Tempo: " << pattern.bpm << " BPM\n";

    return 0;
}
