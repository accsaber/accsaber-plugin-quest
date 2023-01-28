#pragma once
#include <string>
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "Models/AccSaberLeaderboardEntry.hpp"
#include <optional>

namespace AccSaber::Downloaders{
    static const std::string API_URL = "https://api.accsaber.com/";
    static const std::string CDN_URL = "https://cdn.accsaber.com/";
    static const std::string COVERS_ENDPOINT = "covers/";
    static const std::string RANKED_ENDPOINT = "ranked-maps";
    static const std::string LEADERBOARDS_ENDPOINT = "map-leaderboards/";
    static const std::string PLAYERS_ENDPOINT = "players/";
    static const std::string PAGINATION_PAGE = "?page=";
    static const std::string PAGINATION_PAGESIZE = "&pageSize=";
    static const std::string CATEGORY_ENDPOINT = "categories";
    void DownloadLeaderboardAsync(GlobalNamespace::IDifficultyBeatmap* beatmap, int page, std::function<void(std::optional<std::vector<Models::AccSaberLeaderboardEntry>>)> callback);
}