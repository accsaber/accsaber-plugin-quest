#include "Models/AccSaberAPISong.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "Utils/MapUtils.hpp"
#include "Utils/StringUtils.hpp"
#include "main.hpp"

extern AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard;
using namespace AccSaber::Utils;
namespace AccSaber::Models{

    std::optional<AccSaberAPISong> AccSaberAPISong::GetDataForBeatmap(GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap){
        std::string hash = difficultyBeatmap->get_level()->i_IPreviewBeatmapLevel()->get_levelID();
        std::string lowercaseHash = toLower(hash).substr(13);
        std::string diffString = toLower(GetDifficultyName(difficultyBeatmap->get_difficulty()));
        for (auto& song : leaderboard.accSaberRankedSongs){
            if (toLower(song.songHash) == lowercaseHash && toLower(song.difficulty) == diffString) return song;
        }
        return {};
    }

    float AccSaberAPISong::GetComplexityForBeatmap(GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap){
        auto apiSong = GetDataForBeatmap(difficultyBeatmap);
        return apiSong.has_value() ? apiSong.value().complexity : -1.0f;
    }
}