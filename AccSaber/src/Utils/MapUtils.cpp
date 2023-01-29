#include "Utils/MapUtils.hpp"

namespace AccSaber::Utils{
    std::string GetDifficultyName(GlobalNamespace::BeatmapDifficulty difficulty){
        switch (difficulty){
            case GlobalNamespace::BeatmapDifficulty::Easy:
                return "Easy";
            case GlobalNamespace::BeatmapDifficulty::Normal:
                return "Normal";
            case GlobalNamespace::BeatmapDifficulty::Hard:
                return "Hard";
            case GlobalNamespace::BeatmapDifficulty::Expert:
                return "Expert";
            case GlobalNamespace::BeatmapDifficulty::ExpertPlus:
                return "ExpertPlus";
            default: return "";
        }
    }
}