#include "Models/AccSaberAPISong.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "main.hpp"

extern AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard;

namespace AccSaber::Models{

    std::string toLower(std::string s){
        for (char& c : s) c = std::tolower(c);
        return s;
    }

    std::string GetDifficultyString(GlobalNamespace::BeatmapDifficulty diff){
        switch (diff){
            case GlobalNamespace::BeatmapDifficulty::Easy:
                return "easy";
            case GlobalNamespace::BeatmapDifficulty::Normal:
                return "normal";
            case GlobalNamespace::BeatmapDifficulty::Hard:
                return "hard";
            case GlobalNamespace::BeatmapDifficulty::Expert:
                return "expert";
            case GlobalNamespace::BeatmapDifficulty::ExpertPlus:
                return "expertplus";
            default: return "";
        }
    }

    float AccSaberAPISong::GetComplexityForBeatmap(GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap){
        std::string hash = difficultyBeatmap->get_level()->i_IPreviewBeatmapLevel()->get_levelID();
        std::string lowercaseHash = toLower(hash).substr(13);
        std::string diffString = GetDifficultyString(difficultyBeatmap->get_difficulty());
        for (auto& song : leaderboard.accSaberRankedSongs){
            if (toLower(song.songHash) == lowercaseHash && toLower(song.difficulty) == diffString) return song.complexity;
        }
        return -1.0f;
    }
}