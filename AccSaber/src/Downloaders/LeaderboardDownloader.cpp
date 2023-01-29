#include "Downloaders/LeaderboardDownloader.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "Utils/WebRequest.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "Utils/MapUtils.hpp"
#include "main.hpp"

using namespace rapidjson;
using namespace GlobalNamespace;

namespace AccSaber::Downloaders{
    void DownloadLeaderboardAsync(GlobalNamespace::IDifficultyBeatmap* beatmap, int page, std::function<void(std::optional<std::vector<Models::AccSaberLeaderboardEntry>>)> callback){
        std::string beatmapString;
        std::string URL;
        std::string hash = static_cast<std::string>(beatmap->get_level()->i_IPreviewBeatmapLevel()->get_levelID()).substr(13);
        std::string difficulty = Utils::GetDifficultyName(beatmap->get_difficulty());
        std::string characteristic = beatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic()->serializedName;
        beatmapString = string_format("%s/%s/%s", hash.c_str(), characteristic.c_str(), difficulty.c_str());

        URL = API_URL + LEADERBOARDS_ENDPOINT + beatmapString + PAGINATION_PAGE + std::to_string(page) + PAGINATION_PAGESIZE + "10";

        Utils::SendWebRequest(URL, [callback](std::string response){
            std::vector<Models::AccSaberLeaderboardEntry> leaderboard;
            Document document;
            document.Parse(response.c_str());
            if (document.HasMember("errorCode") || document.Empty()) return callback({});
            auto data = document.GetArray();
            for (auto& entry : data){
                auto score = entry.GetObject();
                leaderboard.push_back(Models::AccSaberLeaderboardEntry(
                    score.FindMember("rank")->value.GetInt(), 
                    score.FindMember("playerId")->value.GetInt(), 
                    score.FindMember("playerName")->value.GetString(), 
                    score.FindMember("avatarUrl")->value.GetString(),
                    score.FindMember("accuracy")->value.GetFloat(),
                    score.FindMember("score")->value.GetInt(),
                    score.FindMember("ap")->value.GetDouble(),
                    score.FindMember("timeSet")->value.GetString(),
                    score.FindMember("categoryName")->value.GetString(),
                    score.FindMember("accChamp")->value.GetBool()
                ));
            }
            callback(leaderboard);
        });
    }
}