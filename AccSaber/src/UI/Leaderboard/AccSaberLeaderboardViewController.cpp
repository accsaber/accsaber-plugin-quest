#include "UI/Leaderboard/AccSaberLeaderboardViewController.hpp"
#include "leaderboardcore/shared/LeaderboardCore.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "bsml/shared/BSML.hpp"
#include "assets.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "GlobalNamespace/LoadingControl.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "main.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include <functional>
#include "System/Guid.hpp"
#include "bsml/shared/BSML/Components/ButtonIconImage.hpp"
#include "Downloaders/LeaderboardDownloader.hpp"
#include "GlobalNamespace/LeaderboardTableCell.hpp"
#include "Models/AccSaberAPISong.hpp"
#include "Downloaders/AccSaberDownloader.hpp"
#include "bsml/shared/BSML/Components/ButtonIconImage.hpp"
#include "logging.hpp"
#include "Utils/StringUtils.hpp"

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace BSML::Utilities;
using namespace HMUI;
using namespace GlobalNamespace;

DEFINE_TYPE(AccSaber::UI::Leaderboard, AccSaberLeaderboardViewController);

extern ModInfo modInfo;

extern AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard;
int _lastCell = 0;
int _leaderboardPage = 0;
bool _filterAroundCountry = false;
std::string _currentLeaderboardRefreshId;
int _lastScopeIndex = 0;

namespace AccSaber::UI::Leaderboard
{
    void AccSaberLeaderboardViewController::PostParse(){
        auto icons = ArrayW<IconSegmentedControl::DataItem*>({
            IconSegmentedControl::DataItem::New_ctor(LoadSpriteRaw(IncludedAssets::Global_png), "Global"),
            IconSegmentedControl::DataItem::New_ctor(LoadSpriteRaw(IncludedAssets::Player_png), "Around You")
        });
        scopeSegmentedControl->SetData(icons);

        GetComponentsInChildren<VerticalLayoutGroup*>().First([](auto& v){return v->get_spacing()==-19.4f;})
            ->GetComponentsInChildren<BSML::ButtonIconImage*>(true).copy_to(timePlayedButtons);
        for (auto& icon : timePlayedButtons){
            icon->image->get_rectTransform()->set_localScale({2.5f, 2.5f, 0.0f});
            icon->image->set_raycastTarget(false);
        }
    }

    void AccSaberLeaderboardViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            BSML::parse_and_construct(IncludedAssets::AccSaberLeaderboardViewController_bsml, get_transform(), this);
            leaderboard.get_panelViewController()->set_prompt("Welcome to AccSaber!", 5.0f);
            if (leaderboard.accSaberRankedSongs.size() < 1) Downloaders::DownloadRankedMapsList();
        }
        leaderboard.get_leaderboardViewController()->CheckPage();
        leaderboard.get_leaderboardViewController()->onLeaderboardSet(leaderboard.currentDifficultyBeatmap);

    }

    void AccSaberLeaderboardViewController::CheckPage()
    {
        up_button->set_interactable(_leaderboardPage > 0);
        down_button->set_interactable(_lastScopeIndex == 0);
    }

    void AccSaberLeaderboardViewController::ChangeScope()
    {
        _leaderboardPage = 0;
        onLeaderboardSet(leaderboard.currentDifficultyBeatmap);
        CheckPage();
    }

    void AccSaberLeaderboardViewController::OnIconSelected(IconSegmentedControl* segmentedControl, int index){
        _lastScopeIndex = index;
        ChangeScope();
    }

    void AccSaberLeaderboardViewController::OnPageUp(){
        _leaderboardPage--;
        onLeaderboardSet(leaderboard.currentDifficultyBeatmap);
        CheckPage();
    }

    void AccSaberLeaderboardViewController::OnPageDown(){
        _leaderboardPage++;
        onLeaderboardSet(leaderboard.currentDifficultyBeatmap);
        CheckPage();
    }

    void AccSaberLeaderboardViewController::onLeaderboardSet(IDifficultyBeatmap* difficultyBeatmap){
        DEBUG("refreshing AccSaber leaderboard");
        this->RefreshLeaderboard(difficultyBeatmap, leaderboardTableView, _lastScopeIndex, System::Guid::NewGuid().ToString());
    }

    List<LeaderboardTableView::ScoreData*>* CreateLeaderboardData(std::vector<Models::AccSaberLeaderboardEntry> leaderboard){
        auto tableData = List<LeaderboardTableView::ScoreData*>::New_ctor();
        for (auto& entry : leaderboard) tableData->Add(entry.CreateLeaderboardEntryData());
        return tableData;
    }

    void RichMyText(LeaderboardTableView* tableView){
        for (auto& cell : tableView->GetComponentsInChildren<LeaderboardTableCell*>()){
            cell->playerNameText->set_richText(true);
        }
    }

    void AccSaberLeaderboardViewController::SetLoading(bool value, std::string error){
        leaderboard_loading->set_active(value);
        errorText->get_gameObject()->set_active(!value && error != "");
        if (error == "") return;
        errorText->SetText("blah");
        errorText->SetText(error);
    }

    int getPlayerIndex(std::vector<Models::AccSaberLeaderboardEntry> leaderboardEntires){
        for (int i =0; i<leaderboardEntires.size(); i++){
            if (leaderboardEntires[i].playerId == leaderboard.player.playerId) return i;
        }
        return -1;
    }

    void AccSaberLeaderboardViewController::RefreshLeaderboard(IDifficultyBeatmap* difficultyBeatmap, LeaderboardTableView* tableView,
        PlatformLeaderboardsModel::ScoresScope scope, std::string refreshId) 
    {
        if (!this->isActivated) return;
        tableView->tableView->SetDataSource(nullptr, true);
        SetLoading(true);
        for (auto& button : timePlayedButtons) button->get_gameObject()->set_active(false);
        _currentLeaderboardRefreshId = refreshId;

        std::thread t([difficultyBeatmap, scope, tableView, refreshId, this] {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (_currentLeaderboardRefreshId != refreshId) return;
            Downloaders::DownloadLeaderboardAsync(difficultyBeatmap, scope, _leaderboardPage, [=](std::optional<std::vector<Models::AccSaberLeaderboardEntry>> leaderboardData){
                QuestUI::MainThreadScheduler::Schedule([=]() {
                    if (_currentLeaderboardRefreshId != refreshId) return;
                    if (leaderboardData.has_value()) {
                        if (leaderboardData.value().size() != 0) {
                            int playerIndex = getPlayerIndex(leaderboardData.value());
                            DEBUG("Player Index: {}", playerIndex);  
                            tableView->SetScores(CreateLeaderboardData(leaderboardData.value()), playerIndex);
                            RichMyText(tableView);
                            SetLoading(false);
                            leaderboard.get_panelViewController()->set_color(leaderboardData.value()[0].categoryName);
                            for (int i=0; i<leaderboardData.value().size(); i++){
                                std::string formattedDate = Utils::TimeStampToDateString(leaderboardData.value()[i].timeSet);
                                timePlayedButtons[i]->get_transform()->GetComponentInChildren<HMUI::HoverHint*>()->set_text("Score Set: " + formattedDate);
                                timePlayedButtons[i]->get_gameObject()->SetActive(true);
                            }
                        }
                    }
                    else {
                        auto apiInfo = Models::AccSaberAPISong::GetDataForBeatmap(difficultyBeatmap);
                        if (apiInfo.has_value()){
                            if (_lastScopeIndex == 1){
                                SetLoading(false, leaderboard.player.playerId == "-1" ? "You do not have an account" : "You have not set a score on this leaderboard");
                            }
                            else SetLoading(false, "No more scores for this map");
                            leaderboard.get_panelViewController()->set_color(Utils::toLower(strtok(const_cast<char*>(apiInfo.value().categoryDisplayName.c_str()), " ")));
                        }
                        else{
                            SetLoading(false, "Leaderboard does not exist");
                            leaderboard.get_panelViewController()->set_color("");
                        }
                    }
                    leaderboard.get_panelViewController()->set_complexity(Models::AccSaberAPISong::GetComplexityForBeatmap(difficultyBeatmap));
                });
            });
        });
        t.detach();
    }
}
