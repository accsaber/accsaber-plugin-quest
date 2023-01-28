#pragma once
#include <string>
#include "GlobalNamespace/LeaderboardTableView_ScoreData.hpp"

namespace AccSaber::Models {
    class AccSaberLeaderboardEntry {
        public:
            GlobalNamespace::LeaderboardTableView::ScoreData* CreateLeaderboardEntryData();
            int rank;
            int playerId;
            std::string playerName;
            std::string avatarUrl;
            float accuracy;
            int score;
            double ap;
            std::string timeSet;
            std::string categoryName;
            bool accChamp;

            AccSaberLeaderboardEntry(int rank, int playerId, std::string playerName, std::string avatarUrl, float accuracy, int score, double ap, std::string timeSet, std::string categoryName, bool accChamp) :
            rank(rank),
            playerId(playerId),
            playerName(playerName),
            avatarUrl(avatarUrl),
            accuracy(accuracy),
            score(score),
            ap(ap),
            timeSet(timeSet),
            categoryName(categoryName),
            accChamp(accChamp)
            {}
    };
}