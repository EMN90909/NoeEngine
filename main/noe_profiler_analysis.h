// SPDX-License-Identifier: MIT
// NoeProfiler's dependency-free frame-time analysis policy.
// CPU profiler categories overlap: script time is nested in process time and
// renderer/asset stalls cannot be inferred from CPU-only samples.
#ifndef NOE_PROFILER_ANALYSIS_H
#define NOE_PROFILER_ANALYSIS_H

#include <cmath>

namespace NoeProfiler {
struct FrameSample {
    double frame_seconds = 0.0;
    double process_seconds = 0.0;
    double physics_seconds = 0.0;
    double script_seconds = 0.0;
};

enum class Bottleneck {
    UNKNOWN,
    PROCESS,
    PHYSICS
};

struct FrameAnalysis {
    bool valid = false;
    bool is_spike = false;
    double frame_ms = 0.0;
    double process_ms = 0.0;
    double physics_ms = 0.0;
    double script_ms = 0.0;
    // Lower bound: process and physics could overlap; GPU and stalls are not measured.
    double unattributed_lower_bound_ms = 0.0;
    Bottleneck largest_measured = Bottleneck::UNKNOWN;
};

inline double positive_ms(double seconds) {
    return std::isfinite(seconds) && seconds > 0.0 ? seconds * 1000.0 : 0.0;
}
inline bool is_spike(double frame_seconds, double threshold_ms) {
    return std::isfinite(threshold_ms) && threshold_ms > 0.0 &&
            positive_ms(frame_seconds) >= threshold_ms;
}
inline FrameAnalysis analyze(const FrameSample &sample, double threshold_ms = 33.3) {
    FrameAnalysis result;
    if (!std::isfinite(sample.frame_seconds) || sample.frame_seconds <= 0.0) {
        return result;
    }
    result.valid = true;
    result.frame_ms = positive_ms(sample.frame_seconds);
    result.process_ms = positive_ms(sample.process_seconds);
    result.physics_ms = positive_ms(sample.physics_seconds);
    result.script_ms = positive_ms(sample.script_seconds);
    result.is_spike = is_spike(sample.frame_seconds, threshold_ms);
    // Do not subtract nested script time twice. Do not invent a GPU duration.
    result.unattributed_lower_bound_ms = result.frame_ms > (result.process_ms + result.physics_ms) ?
            result.frame_ms - (result.process_ms + result.physics_ms) : 0.0;
    if (result.process_ms > 0.0 || result.physics_ms > 0.0) {
        result.largest_measured = result.process_ms >= result.physics_ms ? Bottleneck::PROCESS : Bottleneck::PHYSICS;
    }
    return result;
}
inline const char *bottleneck_name(Bottleneck kind) {
    switch (kind) {
        case Bottleneck::PROCESS: return "Process CPU";
        case Bottleneck::PHYSICS: return "Physics CPU";
        case Bottleneck::UNKNOWN: return "Unavailable";
    }
    return "Unavailable";
}
} // namespace NoeProfiler
#endif // NOE_PROFILER_ANALYSIS_H
