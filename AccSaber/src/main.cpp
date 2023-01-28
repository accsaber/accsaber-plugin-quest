#include "main.hpp"
#include "bsml/shared/BSMLDataCache.hpp"
#include "assets.hpp"
#include "custom-types/shared/register.hpp"
#include "questui/shared/QuestUI.hpp"
#include "leaderboardcore/shared/LeaderboardCore.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard;

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    QuestUI::Init();
    LeaderboardCore::Register::RegisterLeaderboard(&leaderboard, modInfo);
    custom_types::Register::AutoRegister();
    getLogger().info("Installing hooks...");
    // Install our hooks (none defined yet)
    getLogger().info("Installed all hooks!");
    
    LeaderboardCore::Events::NotifyLeaderboardSet() += [](GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap){
        leaderboard.currentDifficultyBeatmap = difficultyBeatmap;
        auto vc = leaderboard.get_leaderboardViewController();
        if (vc && vc->isActivated) vc->onLeaderboardSet(difficultyBeatmap);
    };
}

BSML_DATACACHE(accsaber_png) {
    return IncludedAssets::accsaber_png;
}

BSML_DATACACHE(pixel_png) {
    return IncludedAssets::pixel_png;
}

BSML_DATACACHE(carat_png) {
    return IncludedAssets::carat_png;
}

BSML_DATACACHE(download_png) {
    return IncludedAssets::download_png;
}