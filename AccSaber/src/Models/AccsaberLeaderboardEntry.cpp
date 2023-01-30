#include "Models/AccSaberLeaderboardEntry.hpp"

namespace AccSaber::Models {
    GlobalNamespace::LeaderboardTableView::ScoreData* AccSaberLeaderboardEntry::CreateLeaderboardEntryData(){
        std::string formattedScore = string_format(" - (<color=#ffd42a>%.2f%%</color>)", accuracy * 100);
        std::string formattedPP = string_format(" - (<color=#00FFAE>%.2fap</color>)</size>", ap);
        std::string result = "<size=80%>" + playerName + formattedScore + formattedPP + "</size>";
        return GlobalNamespace::LeaderboardTableView::ScoreData::New_ctor(score, result, rank, false);
    }
}