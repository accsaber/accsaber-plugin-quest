#pragma once
#include <string>

namespace AccSaber::Models {
    struct AccSaberUserModel {
        int rank = 0;
        std::string playerId = "-1";
        std::string playerName;
        std::string hmd;
        float averageAcc;
        float ap = -1.0f;
        float averageApPerMap;
        int rankedPlays;
        bool accChamp;
        AccSaberUserModel(int rank, std::string playerId, std::string playerName, std::string hmd, 
            float averageAcc, float ap, float averageApPerMap, int rankedPlays, bool accChamp) :
                rank(rank),
                playerId(playerId),
                playerName(playerName),
                hmd(hmd),
                averageAcc(averageAcc),
                ap(ap),
                averageApPerMap(averageApPerMap),
                rankedPlays(rankedPlays),
                accChamp(accChamp)
                {}
        AccSaberUserModel(){}
    };
}