#pragma once

#include "leaderboardcore/shared/Models/CustomLeaderboard.hpp"
#include "UI/Leaderboard/AccSaberLeaderboardViewController.hpp"
#include "UI/Leaderboard/AccSaberPanel.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "Models/AccSaberAPISong.hpp"
#include "Models/AccSaberUserModel.hpp"

namespace AccSaber::UI::Leaderboard
{
    class CustomLeaderboard final : public LeaderboardCore::Models::CustomLeaderboard
    {
        public:
            AccSaberPanel* get_panelViewController() final override;
            AccSaberLeaderboardViewController* get_leaderboardViewController() final override;
            GlobalNamespace::IDifficultyBeatmap* currentDifficultyBeatmap;
            std::vector<Models::AccSaberAPISong> accSaberRankedSongs;
            // 0 = overall, 1 = standard, 2 = true, 3 = tech
            std::array<Models::AccSaberUserModel, 4> playerCategoryData;

        private:
            SafePtrUnity<AccSaberPanel> panelViewController;
            SafePtrUnity<AccSaberLeaderboardViewController> leaderboardViewController;
    };
}