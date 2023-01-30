#pragma once
#include <string>
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "Models/AccSaberLeaderboardEntry.hpp"
#include <optional>

namespace AccSaber::Downloaders{
    void DownloadLeaderboardAsync(GlobalNamespace::IDifficultyBeatmap* beatmap, int scope, int page, std::function<void(std::optional<std::vector<Models::AccSaberLeaderboardEntry>>)> callback);
}