#pragma once
#include <string>
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include <optional>

namespace AccSaber::Models{
    class AccSaberAPISong {
        public:
            std::string songName;
            std::string songSubName;
            std::string songAuthorName;
            std::string levelAuthorName;
            std::string difficulty;
            std::string beatSaverKey;
            std::string songHash;
            float complexity;
            std::string categoryDisplayName;
            AccSaberAPISong(std::string songName, std::string songSubName, std::string songAuthorName, std::string levelAuthorName, 
                std::string difficulty, std::string beatSaverKey, std::string songHash, float complexity, std::string categoryDisplayName) :
                    songName(songName),
                    songSubName(songSubName),
                    songAuthorName(songAuthorName),
                    levelAuthorName(levelAuthorName),
                    difficulty(difficulty),
                    beatSaverKey(beatSaverKey),
                    songHash(songHash),
                    complexity(complexity),
                    categoryDisplayName(categoryDisplayName)
                    {}
            AccSaberAPISong();
            static float GetComplexityForBeatmap(GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap);
            static std::optional<AccSaberAPISong> GetDataForBeatmap(GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap);
    };
}