#include "UI/Leaderboard/AccSaberLeaderboardViewController.hpp"
#include "leaderboardcore/shared/LeaderboardCore.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "bsml/shared/BSML.hpp"
#include "assets.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "GlobalNamespace/LoadingControl.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "main.hpp"
// #include "CustomTypes/Components/LeaderboardScoreInfoButtonHandler.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include <functional>
#include "System/Guid.hpp"
#include "bsml/shared/BSML/Components/ButtonIconImage.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "Downloaders/LeaderboardDownloader.hpp"
#include "GlobalNamespace/StandardLevelDetailViewController.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/LeaderboardTableCell.hpp"
#include "Models/AccSaberAPISong.hpp"
#include "Downloaders/AccSaberDownloader.hpp"

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
// using namespace AccSaber::Services;
using namespace BSML::Utilities;
using namespace HMUI;
using namespace GlobalNamespace;

DEFINE_TYPE(AccSaber::UI::Leaderboard, AccSaberLeaderboardViewController);

extern ModInfo modInfo;

extern AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard;
// AccSaber::CustomTypes::Components::LeaderboardScoreInfoButtonHandler* leaderboardScoreInfoButtonHandler;
int _lastCell = 0;
int _leaderboardPage = 0;
bool _filterAroundCountry = false;
std::string _currentLeaderboardRefreshId;
int _lastScopeIndex = 0;

namespace AccSaber::UI::Leaderboard
{

    void AccSaberLeaderboardViewController::PostParse(){
        Array<IconSegmentedControl::DataItem*>* array = ::Array<IconSegmentedControl::DataItem*>::New({
            IconSegmentedControl::DataItem::New_ctor(LoadSpriteRaw(IncludedAssets::Global_png), "Global"),
            IconSegmentedControl::DataItem::New_ctor(LoadSpriteRaw(IncludedAssets::Player_png), "Around You")
        });
        scopeSegmentedControl->SetData(array);
        CreateLoadingControl();
    }

    void AccSaberLeaderboardViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            BSML::parse_and_construct(IncludedAssets::AccSaberLeaderboardViewController_bsml, get_transform(), this);
            leaderboard.get_panelViewController()->Prompt("Welcome to AccSaber!", false, 5.0f, nullptr);
            if (leaderboard.accSaberRankedSongs.size() < 1) AccSaber::Downloaders::DownloadRankedMapsList();

            std::thread([] {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                QuestUI::MainThreadScheduler::Schedule([]() {
                    leaderboard.get_leaderboardViewController()->CheckPage();
                    leaderboard.get_leaderboardViewController()->onLeaderboardSet(leaderboard.currentDifficultyBeatmap);
                });
            }).detach();
        }
        else{
            leaderboard.get_leaderboardViewController()->CheckPage();
            leaderboard.get_leaderboardViewController()->onLeaderboardSet(leaderboard.currentDifficultyBeatmap);
        }
    }

    void AccSaberLeaderboardViewController::CheckPage()
    {
        auto* btn = up_button->get_transform()->GetComponentInChildren<UnityEngine::UI::Button*>();
        if (_leaderboardPage > 0) btn->set_interactable(true);
        else btn->set_interactable(false);
    }

    void AccSaberLeaderboardViewController::ChangeScope()
    {
        if (this->isActivated)
        {
            _leaderboardPage = 0;
            onLeaderboardSet(leaderboard.currentDifficultyBeatmap);
            CheckPage();
        }
    }

    void SetErrorState(LoadingControl* loadingControl, std::string errorText, bool showRefreshButton = false)
    {
        loadingControl->Hide();
        loadingControl->ShowText("literallyunalivemenow", false);
        loadingControl->ShowText(errorText, showRefreshButton);
    }

    void LoadingControl_ShowLoading(LoadingControl* loadingControl){
        loadingControl->get_gameObject()->set_active(true);
        loadingControl->loadingContainer->SetActive(true);
        loadingControl->refreshContainer->SetActive(false);
        loadingControl->refreshText->SetText("");
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
        auto* view = leaderboardTableView->get_transform()->GetComponentInChildren<LeaderboardTableView*>();
        auto* loadingControl = leaderboardTableView->GetComponent<LoadingControl*>();
        this->RefreshLeaderboard(difficultyBeatmap, view, 0, loadingControl, System::Guid::NewGuid().ToString());
    }

    List<LeaderboardTableView::ScoreData*>* CreateLeaderboardData(std::vector<Models::AccSaberLeaderboardEntry> leaderboard){
        auto tableData = List<LeaderboardTableView::ScoreData*>::New_ctor();
        for (auto& entry : leaderboard) tableData->Add(entry.CreateLeaderboardEntryData());
        return tableData;
    }

    void RichMyText(LeaderboardTableView* tableView){
        for (auto& cell : tableView->get_transform()->GetComponentsInChildren<LeaderboardTableCell*>()){
            cell->playerNameText->set_richText(true);
        }
    }

    void AccSaberLeaderboardViewController::RefreshLeaderboard(IDifficultyBeatmap* difficultyBeatmap, LeaderboardTableView* tableView,
        PlatformLeaderboardsModel::ScoresScope scope, LoadingControl* loadingControl, std::string refreshId) 
    {
        if (!this->isActivated) return;
        tableView->tableView->SetDataSource(nullptr, true);
        LoadingControl_ShowLoading(loadingControl);

        _currentLeaderboardRefreshId = refreshId;

        std::thread t([difficultyBeatmap, scope, loadingControl, tableView, refreshId, this] {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (_currentLeaderboardRefreshId != refreshId) return;
            Downloaders::DownloadLeaderboardAsync(difficultyBeatmap, _leaderboardPage, [=](std::optional<std::vector<Models::AccSaberLeaderboardEntry>> leaderboardData){
                QuestUI::MainThreadScheduler::Schedule([=]() {
                    if (_currentLeaderboardRefreshId != refreshId) return;
                    if (leaderboardData.has_value()) {
                        if (leaderboardData.value().size() != 0) {   
                            tableView->SetScores(CreateLeaderboardData(leaderboardData.value()), -1);
                            RichMyText(tableView);
                            loadingControl->ShowText(System::String::_get_Empty(), false);
                            leaderboard.get_panelViewController()->set_color(leaderboardData.value()[0].categoryName);
                        }
                    }
                    else {
                        SetErrorState(loadingControl, "Leaderboard does not exist");
                        leaderboard.get_panelViewController()->set_color("");
                    }
                    leaderboard.get_panelViewController()->set_complexity(
                        Models::AccSaberAPISong::GetComplexityForBeatmap(difficultyBeatmap));
                });
            });
        });
        t.detach();
    }

    void AccSaberLeaderboardViewController::CreateLoadingControl(){
        Object::Destroy(leaderboardTableView->GetComponent<LoadingControl*>());
        auto* loadingTemplate = UnityEngine::Resources::FindObjectsOfTypeAll<PlatformLeaderboardViewController*>()
            .FirstOrDefault()->get_transform()->Find("Container/LeaderboardTableView/LoadingControl")->GetComponentInChildren<LoadingControl*>();
        leaderboardTableView->AddComponent<LoadingControl*>()->Instantiate(loadingTemplate, leaderboardTableView->get_transform(), false);
        auto* loadingControl = leaderboardTableView->GetComponent<LoadingControl*>();
        Object::Destroy(loadingControl->get_transform()->GetComponentInChildren<TMPro::TextMeshProUGUI*>());
        loadingControl->loadingContainer = leaderboard_loading;
        loadingControl->downloadingContainer = leaderboard_loading;
        loadingControl->refreshContainer = Object::Instantiate(loadingTemplate->refreshContainer, leaderboardTableView->get_transform(), false);
        loadingControl->refreshContainer->get_transform()->set_localPosition(leaderboard_loading->get_transform()->get_localPosition());
        loadingControl->refreshText = loadingControl->get_transform()->GetComponentsInChildren<TMPro::TextMeshProUGUI*>().get(1);
        loadingControl->refreshButton = Object::Instantiate(loadingTemplate->refreshButton, leaderboardTableView->get_transform(), false);
        loadingControl->refreshButton->get_gameObject()->SetActive(false);
        loadingControl->refreshContainer->SetActive(false);
        GameObject::Destroy(loadingControl->refreshText->GetComponent<Polyglot::LocalizedTextMeshProUGUI*>());
    }
}
