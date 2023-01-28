#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "UI/Leaderboard/AccSaberLeaderboardViewController.hpp"
#include "UI/Leaderboard/AccSaberPanel.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "bsml/shared/Helpers/creation.hpp"
#include "main.hpp"

namespace AccSaber::UI::Leaderboard
{
    AccSaberPanel* CustomLeaderboard::get_panelViewController()
    {
        if (!panelViewController || !panelViewController.isAlive())
        {
            panelViewController = BSML::Helpers::CreateViewController<AccSaber::UI::Leaderboard::AccSaberPanel*>();
        }
        return panelViewController.ptr();
    }

    AccSaberLeaderboardViewController* CustomLeaderboard::get_leaderboardViewController()
    {
        if (!leaderboardViewController || !leaderboardViewController.isAlive())
        {
            leaderboardViewController = BSML::Helpers::CreateViewController<AccSaber::UI::Leaderboard::AccSaberLeaderboardViewController*>();
        }
        return leaderboardViewController.ptr();
    }
}