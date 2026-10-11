
#include "audio_engine.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <limits>
#include <random>
#include <vector>

namespace paul {
namespace {

constexpr double PI = 3.14159265358979323846;

void write_u16(std::ofstream& out, std::uint16_t value) {
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
}

void write_u32(std::ofstream& out, std::uint32_t value) {
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
}

void mix_tone(
    std::vector<float>& audio,
    int start,
    int rate,
    double duration,
    double frequency,
    double end_frequency,
    float amplitude
) {
    const int count = std::min(
        static_cast<int>(duration * rate),
        static_cast<int>(audio.size()) - start
    );

    for (int i = 0; i < count; ++i) {
        const double t = static_cast<double>(i) / rate;
        const double progress = t / duration;
        const double freq =
            frequency + (end_frequency - frequency) * progress;
        const double envelope = std::exp(-8.0 * t);

        audio[start + i] += static_cast<float>(
            amplitude * envelope * std::sin(2.0 * PI * freq * t)
        );
    }
}

void mix_noise(
    std::vector<float>& audio,
    int start,
    int rate,
    double duration,
    float amplitude,
    std::mt19937& rng
) {
    const int count = std::min(
        static_cast<int>(duration * rate),
        static_cast<int>(audio.size()) - start
    );

    std::uniform_real_distribution<float> noise(-1.0f, 1.0f);

    for (int i = 0; i < count; ++i) {
        const double t = static_cast<double>(i) / rate;
        const double envelope = std::exp(-18.0 * t);
        audio[start + i] += amplitude * static_cast<float>(envelope)
            * noise(rng);
    }
}

} // namespace

bool render_wav(
    const RhythmPattern& pattern,
    const std::string& filename,
    int sample_rate
) {
    if (sample_rate < 8000 || sample_rate > 192000 ||
        pattern.bpm < 40 || pattern.bpm > 240 ||
        pattern.steps_per_bar <= 0 ||
        pattern.steps_per_bar > 64) {
        return false;
    }

    const double beat_seconds = 60.0 / pattern.bpm;
    const double bar_seconds =
        beat_seconds * pattern.steps_per_bar / 4.0;
    const double duration = bar_seconds * 2.0;

    const double max_samples =
        duration * sample_rate;

    if (max_samples <= 0.0 || max_samples > 10000000.0) {
        return false;
    }

    std::vector<float> audio(
        static_cast<std::size_t>(max_samples) + sample_rate / 2,
        0.0f
    );

    std::mt19937 rng(12345);

    for (int repeat = 0; repeat < 2; ++repeat) {
        for (const auto& hit : pattern.hits) {
            if (hit.step < 0 || hit.step >= pattern.steps_per_bar ||
                !std::isfinite(hit.velocity) ||
                hit.velocity < 0.0f || hit.velocity > 1.0f) {
                return false;
            }

            const double step_seconds =
                bar_seconds * hit.step / pattern.steps_per_bar;
            const int start = static_cast<int>(
                (repeat * bar_seconds + step_seconds) * sample_rate
            );

            const float velocity = hit.velocity;

            switch (hit.instrument) {
                case DrumInstrument::Kick:
                    mix_tone(audio, start, sample_rate, 0.28,
                             150.0, 45.0, 0.9f * velocity);
                    break;

                case DrumInstrument::Snare:
                    mix_noise(audio, start, sample_rate, 0.20,
                             0.55f * velocity, rng);
                    mix_tone(audio, start, sample_rate, 0.12,
                             190.0, 100.0, 0.25f * velocity);
                    break;

                case DrumInstrument::ClosedHiHat:
                    mix_noise(audio, start, sample_rate, 0.055,
                             0.22f * velocity, rng);
                    break;
            }
        }
    }

    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        return false;
    }

    const std::uint32_t data_size =
        static_cast<std::uint32_t>(audio.size() * 2);
    const std::uint32_t byte_rate =
        static_cast<std::uint32_t>(sample_rate * 2);

    out.write("RIFF", 4);
    write_u32(out, 36 + data_size);
    out.write("WAVE", 4);
    out.write("fmt ", 4);
    write_u32(out, 16);
    write_u16(out, 1);
    write_u16(out, 1);
    write_u32(out, static_cast<std::uint32_t>(sample_rate));
    write_u32(out, byte_rate);
    write_u16(out, 2);
    write_u16(out, 16);
    out.write("data", 4);
    write_u32(out, data_size);

    for (float sample : audio) {
        sample = std::max(-1.0f, std::min(1.0f, sample));
        const auto value = static_cast<std::int16_t>(
            sample * 32767.0f
        );
        write_u16(out, static_cast<std::uint16_t>(value));
    }

    return static_cast<bool>(out);
}

} // namespace paul
