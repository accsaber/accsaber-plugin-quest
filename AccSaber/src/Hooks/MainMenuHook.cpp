#include "hooking.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "modloader/shared/modloader.hpp"
#include "Utils/StringUtils.hpp"
#include "Downloaders/PlayerDownloader.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "logging.hpp"
#include "main.hpp"

extern AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard; 

using namespace GlobalNamespace;

MAKE_AUTO_HOOK_MATCH(MainMenuViewController_DidActivate, &MainMenuViewController::DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (!firstActivation) return;
    if (!Modloader::requireMod("ScoreSaber")) return;
    std::string scary = readfile("/sdcard/ModData/com.beatgames.beatsaber/Mods/ScoreSaber/scoresaber_DO_NOT_SHARE.scary");
    if (!scary.empty()){
        std::string playerId = AccSaber::Utils::split(scary, ':')[1];
        AccSaber::Downloaders::DownloadPlayerInfoAsync(playerId, [](std::optional<AccSaber::Models::AccSaberUserModel> player){
            if (player.has_value()){
                leaderboard.player = player.value();
                if (leaderboard.get_panelViewController()->wasActivatedBefore){
                    leaderboard.get_panelViewController()->set_ranking(leaderboard.player.rank, leaderboard.player.ap);
                }
            }
        });
    }
}