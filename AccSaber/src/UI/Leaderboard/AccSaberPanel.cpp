#include "UI/Leaderboard/AccSaberPanel.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/Touchable.hpp"
#include "assets.hpp"
#include "bsml/shared/BSML.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "bsml/shared/Helpers/getters.hpp"
#include "bsml/shared/Helpers/creation.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/Time.hpp"
#include "main.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "Models/AccSaberAPISong.hpp"
#include "Utils/StringUtils.hpp"
#include "logging.hpp"

DEFINE_TYPE(AccSaber::UI::Leaderboard, AccSaberPanel);

using namespace UnityEngine;

extern AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard;

namespace AccSaber::UI::Leaderboard
{
    void AccSaberPanel::set_color(UnityEngine::Color color){
        bgImage->set_color(color);
    }

    void AccSaberPanel::set_color(std::string category){
        if (category == "standard") set_color(standardAccColor);
        else if (category == "tech") set_color(techAccColor);
        else if (category == "true") set_color(trueAccColor);
        else set_color(Color(0, 0, 0, 1));
    }

    void AccSaberPanel::set_complexity(float complexity)
    {
        leaderboard_ranked_text->SetText(string_format("<b><color=#EDFF55>Map Complexity: </color></b> %.2f", complexity));
        set_loading(false);
    }

    void AccSaberPanel::set_ranking(int rank, float ap)
    {
        const char* category = overallRankingScope ? "Overall" : "Category";
        const char* error = rank == -1 ? "No rank recorded for player" : "Not an AccSaber ranked map";
        if (rank < 0) global_rank_text->SetText(string_format("<b><color=#EDFF55>%s Ranking: %s", category, error));
        else global_rank_text->SetText(string_format("<b><color=#EDFF55>%s Ranking: </color></b>#%d<size=3> (<color=#00FFAE>%.2fap</color></size>)", category, rank, ap));
        set_loading(false);
    }

    void AccSaberPanel::PostParse(){
        bgImage = container->GetComponent<BSML::Backgroundable*>()->background;
        bgImage->gradient = true;
        accsaber_logo->skew = 0.18f;
        separator->skew = 0.18f;

        overallRankingScope = true;
        set_color("");
        set_ranking(leaderboard.playerCategoryData[0].rank, leaderboard.playerCategoryData[0].ap);
        set_complexity(-1);
    }

    void AccSaberPanel::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::PanelView_bsml, this->get_transform(), this);
    }

    void AccSaberPanel::set_prompt(std::string text, int dismissTime)
    {
        promptText->set_text(text);
        if (dismissTime != -1)
        {
            std::thread t([dismissTime, this] {
                std::this_thread::sleep_for(std::chrono::seconds(dismissTime));
                QuestUI::MainThreadScheduler::Schedule([=]() {
                    this->promptText->set_text(std::string());
                });
            });
            t.detach();
        }
    }

    void AccSaberPanel::set_loading(bool value)
    {
        prompt_loader->get_gameObject()->SetActive(value);
        global_rank_text->get_gameObject()->SetActive(!value);
        leaderboard_ranked_text->get_gameObject()->SetActive(!value);
    }

    void AccSaberPanel::downloadClick(){
        // HACK: Resources call to get these objects to use for changing menu is bad but there's no good other choice afaik
        auto mainfc = BSML::Helpers::GetMainFlowCoordinator();
        auto youngest = mainfc->YoungestChildFlowCoordinatorOrSelf();

        // auto fc = Resources::FindObjectsOfTypeAll<AccSaber::UI::FlowCoordinators::AccSaberFlowCoordinator*>().FirstOrDefault();
        // if (!fc)
        // {
        //     fc = BSML::Helpers::CreateFlowCoordinator<AccSaber::UI::FlowCoordinators::AccSaberFlowCoordinator*>();
        // }

        // youngest->PresentFlowCoordinator(fc, nullptr, ViewController::AnimationDirection::Horizontal, false, false);
    }

    void AccSaberPanel::set_ranking_category(std::string category){
        if (overallRankingScope) return set_ranking(leaderboard.playerCategoryData[0].rank, leaderboard.playerCategoryData[0].ap);
        Models::AccSaberUserModel* userModel;
        if (category == "standard") userModel = &leaderboard.playerCategoryData[1];
        else if (category == "true") userModel = &leaderboard.playerCategoryData[2];
        else if (category == "tech") userModel = &leaderboard.playerCategoryData[3];
        else return set_ranking(-2, -1.0f);
        set_ranking(userModel->rank, userModel->ap);
    }

    void AccSaberPanel::OnRankingClick(){
        overallRankingScope = !overallRankingScope;
        auto mapData = Models::AccSaberAPISong::GetDataForBeatmap(leaderboard.currentDifficultyBeatmap);
        std::string category = mapData.has_value() ? Utils::toLower(Utils::split(mapData.value().categoryDisplayName, ' ')[0]) : "";
        set_ranking_category(category);
    }
}
