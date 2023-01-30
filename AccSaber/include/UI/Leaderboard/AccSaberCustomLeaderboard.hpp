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
            Models::AccSaberUserModel player;

        private:
            SafePtrUnity<AccSaberPanel> panelViewController;
            SafePtrUnity<AccSaberLeaderboardViewController> leaderboardViewController;
    };
}