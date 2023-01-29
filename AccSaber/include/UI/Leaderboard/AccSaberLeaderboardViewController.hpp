#pragma once

#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"
#include "bsml/shared/macros.hpp"
#include "GlobalNamespace/LeaderboardTableView.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/IconSegmentedControl_DataItem.hpp"
#include "HMUI/IconSegmentedControl.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_ScoresScope.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "bsml/shared/BSML/Components/ButtonIconImage.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

DECLARE_CLASS_CODEGEN(AccSaber::UI::Leaderboard, AccSaberLeaderboardViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::ViewController::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, leaderboardTableView);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, leaderboard_loading);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, errorText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, up_button);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, down_button);
    DECLARE_INSTANCE_FIELD(HMUI::IconSegmentedControl*, scopeSegmentedControl);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, OnIconSelected, HMUI::IconSegmentedControl* segmentedControl, int index);
    DECLARE_INSTANCE_METHOD(void, OnPageUp);
    DECLARE_INSTANCE_METHOD(void, OnPageDown);

    public:
    void RefreshLeaderboard(GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::LeaderboardTableView* tableView,
                            GlobalNamespace::PlatformLeaderboardsModel::ScoresScope scope,
                            std::string refreshId);
    void onLeaderboardSet(GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap);
    void ChangeScope();
    void CheckPage();
    private:
        std::vector<BSML::ButtonIconImage*> timePlayedButtons;
        void SetLoading(bool value, std::string error = "");
)
