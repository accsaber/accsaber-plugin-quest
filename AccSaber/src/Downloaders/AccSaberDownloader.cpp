#include "Downloaders/AccSaberDownloader.hpp"
#include "Utils/WebRequest.hpp"
#include <vector>
#include "Models/AccSaberAPISong.hpp"
#include "UI/Leaderboard/AccSaberCustomLeaderboard.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "Utils/Constants.hpp"

extern AccSaber::UI::Leaderboard::CustomLeaderboard leaderboard;

using namespace rapidjson;
using namespace AccSaber::Utils::Constants;

namespace AccSaber::Downloaders{
    void DownloadRankedMapsList(){
        Utils::SendWebRequest(API_URL + RANKED_ENDPOINT, [](std::string response){
            Document document;
            document.Parse(response.c_str());
            if (document.HasMember("errorCode") || document.Empty()) return;
            auto data = document.GetArray();
            for (auto& entry : data){
                auto score = entry.GetObject();
                leaderboard.accSaberRankedSongs.push_back(Models::AccSaberAPISong(
                    score.FindMember("songName")->value.GetString(), 
                    score.FindMember("songSubName")->value.GetString(), 
                    score.FindMember("songAuthorName")->value.GetString(), 
                    score.FindMember("levelAuthorName")->value.GetString(),
                    score.FindMember("difficulty")->value.GetString(),
                    score.FindMember("beatSaverKey")->value.GetString(),
                    score.FindMember("songHash")->value.GetString(),
                    score.FindMember("complexity")->value.GetFloat(),
                    score.FindMember("categoryDisplayName")->value.GetString()
                ));
            }
        });
    }
}