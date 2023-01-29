#pragma once

#include <string>
#include "GlobalNamespace/BeatmapDifficulty.hpp"

namespace AccSaber::Utils{
    std::string GetDifficultyName(GlobalNamespace::BeatmapDifficulty difficulty);
}