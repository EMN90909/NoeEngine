// SPDX-License-Identifier: MIT
#include "main/noe_profiler_analysis.h"
#include <cassert>
#include <cmath>
#include <limits>
int main() {
    const NoeProfiler::FrameSample slow{0.045, 0.021, 0.013, 0.011};
    const auto a = NoeProfiler::analyze(slow);
    assert(a.valid && a.is_spike);
    assert(std::abs(a.frame_ms - 45.0) < 1e-6);
    assert(std::abs(a.unattributed_lower_bound_ms - 11.0) < 1e-6);
    assert(a.largest_measured == NoeProfiler::Bottleneck::PROCESS);
    assert(a.script_ms == 11.0);
    assert(!NoeProfiler::is_spike(0.016, 33.3));
    assert(NoeProfiler::is_spike(0.0333, 33.3));
    assert(!NoeProfiler::is_spike(0.045, 0.0));
    assert(!NoeProfiler::is_spike(std::numeric_limits<double>::quiet_NaN(), 33.3));
    assert(!NoeProfiler::analyze({-1.0, 1.0, 0.0, 0.0}).valid);
    assert(NoeProfiler::analyze({0.02, 0.018, 0.018, 0.0}).unattributed_lower_bound_ms == 0.0);
    assert(NoeProfiler::analyze({0.02, 0.0, 0.011, 0.0}).largest_measured == NoeProfiler::Bottleneck::PHYSICS);
    return 0;
}
