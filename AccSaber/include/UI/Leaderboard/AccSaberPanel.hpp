#pragma once

#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"
#include "bsml/shared/macros.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "bsml/shared/BSML/Components/ClickableImage.hpp"
#include "bsml/shared/BSML/Components/ClickableText.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/GameObject.hpp"

DECLARE_CLASS_CODEGEN(AccSaber::UI::Leaderboard, AccSaberPanel, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::ViewController::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, container);
    DECLARE_INSTANCE_FIELD(BSML::ClickableImage*, accsaber_logo);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, separator);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, global_rank_text);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, leaderboard_ranked_text);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, promptText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, prompt_loader);
    DECLARE_INSTANCE_METHOD(void, OnLogoClick);
    DECLARE_INSTANCE_METHOD(void, OnRankTextClick);
    DECLARE_INSTANCE_METHOD(void, OnRankedStatusClick);
    DECLARE_INSTANCE_METHOD(void, Update);

    public:
        int scoreboardId;
        void set_ranking(int rank, float pp);
        void set_color(UnityEngine::Color color);
        void set_color(std::string category);
        void set_complexity(float complexity);
        void Prompt(std::string status, bool loadingIndicator, float dismiss, std::function<void()> callback);
        void set_prompt(std::string text, int dismissTime);
        custom_types::Helpers::Coroutine SetPrompt(std::string status, bool showIndicator, float dismiss, std::function<void()> callback);
        void set_loading(bool value);
    private:
        bool rainbow = false;
        bool wasRainbow = false;
        float colorAngle = 0.0f;
        static constexpr const UnityEngine::Color trueAccColor = {0.015f, 0.906f, 0.176f, 1.0f};
        static constexpr const UnityEngine::Color standardAccColor = {0.039f, 0.573f, 0.918f, 1.0f};
        static constexpr const UnityEngine::Color techAccColor = {0.902f, 0.027f, 0.027f, 1.0f};
)
