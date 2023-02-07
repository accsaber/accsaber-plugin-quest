#include "hooking.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "modloader/shared/modloader.hpp"
#include "Utils/StringUtils.hpp"
#include "Downloaders/PlayerDownloader.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "Utils/Constants.hpp"
#include "logging.hpp"
#include "main.hpp"

extern AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard; 

using namespace GlobalNamespace;
using namespace AccSaber::Utils::Constants;

MAKE_AUTO_HOOK_MATCH(MainMenuViewController_DidActivate, &MainMenuViewController::DidActivate, void, MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (!firstActivation) return;
    if (!Modloader::requireMod("ScoreSaber")) return;
    std::string scary = readfile("/sdcard/ModData/com.beatgames.beatsaber/Mods/ScoreSaber/scoresaber_DO_NOT_SHARE.scary");
    if (!scary.empty()){
        std::string playerId = AccSaber::Utils::split(scary, ':')[1];
        std::string categories[] = {OVERALL, STANDARD, TRUE, TECH};
        for (int i=0; i<leaderboard.playerCategoryData.size(); i++){
            AccSaber::Downloaders::DownloadPlayerInfoAsync(playerId, categories[i], [i](std::optional<AccSaber::Models::AccSaberUserModel> player){
                if (player.has_value()){
                    leaderboard.playerCategoryData[i] = player.value();
                    if (i == 0 && leaderboard.get_panelViewController()->wasActivatedBefore){
                        leaderboard.get_panelViewController()->set_ranking(leaderboard.playerCategoryData[0].rank, leaderboard.playerCategoryData[0].ap);
                    }
                }
            });
        }
    }
}