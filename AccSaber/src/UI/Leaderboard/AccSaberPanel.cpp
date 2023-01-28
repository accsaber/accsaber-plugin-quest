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

DEFINE_TYPE(AccSaber::UI::Leaderboard, AccSaberPanel);

using namespace UnityEngine;

HMUI::ImageView* bgImage;

namespace AccSaber::UI::Leaderboard
{
    void AccSaberPanel::set_color(UnityEngine::Color color){
        bgImage->set_color(color);
    }

    void AccSaberPanel::set_color(std::string category){
        if (category == "standard") set_color(standardAccColor);
        else if (category == "tech") set_color(techAccColor);
        else if (category == "true") set_color(trueAccColor);
        else set_color(UnityEngine::Color(0, 0, 0, 1));
    }

    void AccSaberPanel::set_complexity(float complexity)
    {
        leaderboard_ranked_text->SetText(string_format("<b><color=#EDFF55>Map Complexity: </color></b> %.2f", complexity));
        set_loading(false);
    }

    void AccSaberPanel::set_ranking(int rank, float pp)
    {
        global_rank_text->SetText(string_format("<b><color=#EDFF55>Global Ranking: </color></b>#%d<size=3> (<color=#00FFAE>%.2fap</color></size>)", rank, pp));
        set_loading(false);
    }

    void AccSaberPanel::PostParse(){
        bgImage = container->GetComponent<BSML::Backgroundable*>()->background;
        bgImage->skew = 0.18f;
        bgImage->gradient = true;
        accsaber_logo->skew = 0.18f;
        separator->skew = 0.18f;

        // rainbow=true;
        
        // currently just here for POC
        set_color(techAccColor);
        set_ranking(1, 123.45f);
        set_complexity(-1);
    }

    void AccSaberPanel::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::PanelView_bsml, this->get_transform(), this);
    }

    void AccSaberPanel::Prompt(std::string status, bool loadingIndicator, float dismiss, std::function<void()> callback)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
        custom_types::Helpers::CoroutineHelper::New(SetPrompt(status, loadingIndicator, dismiss, callback)));
    }

    custom_types::Helpers::Coroutine AccSaberPanel::SetPrompt(std::string status, bool showIndicator, float dismiss, std::function<void()> callback)
    {
        this->promptText->SetText(status);

        std::string text = status;

        for (int i = 1; i < (dismiss * 2) + 1; i++)
        {
            co_yield reinterpret_cast<System::Collections::IEnumerator*>(
                CRASH_UNLESS(WaitForSeconds::New_ctor(0.5f)));

            // Couldn't get the loading indicator to work so right now it just displays
            // dots as the loading indicator
            if (showIndicator)
            {
                if (i % 4 != 0)
                {
                    text = text + ".";
                    promptText->SetText(text);
                }
                else
                {
                    for (int k = 0; k < 3; k++)
                    {
                        text.pop_back();
                    }
                    promptText->SetText(text);
                }
            }
        }

        if (dismiss > 0)
        {
            promptText->set_text(std::string());
        }

        if (callback)
        {
            callback();
        }

        co_return;
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

    void AccSaberPanel::OnLogoClick(){
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

    void AccSaberPanel::OnRankTextClick(){
        // just make sure to have this actually assigned
        // if (playerProfileModal && Object::IsNativeObjectAlive(playerProfileModal))
        // {
        //     playerProfileModal->Show(AccSaber::Services::PlayerService::playerInfo.localPlayerData.id);
        // }
    }

    void AccSaberPanel::OnRankedStatusClick(){
        auto songURL = string_format("https://accsaber.com/leaderboard/%d", scoreboardId);
        Application::OpenURL(songURL);
    }

    void AccSaberPanel::Update()
    {
    //     // if we are not going to be rainbow, and we were just rainbow
    //     if (!rainbow && wasRainbow)
    //     {
    //         // set bg color to default
    //         wasRainbow = false;
    //         set_color(defaultColor);
    //     }
    //     else if (rainbow)
    //     {
    //         wasRainbow = true;
    //         // make this color dependent on some kind of rainbow / gradient source
    //         colorAngle += UnityEngine::Time::get_deltaTime() * 0.1f;
    //         colorAngle = std::fmod(colorAngle, 1.0f);
    //         UnityEngine::Color color = UnityEngine::Color::HSVToRGB(colorAngle, 1.0f, 1.0f);
    //         set_color(color);
    //     }
    }
}
