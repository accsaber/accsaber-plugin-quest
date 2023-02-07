#include "Downloaders/PlayerDownloader.hpp"
#include "Utils/WebRequest.hpp"
#include "Utils/Constants.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "Models/AccSaberUserModel.hpp"
#include <vector>
#include "logging.hpp"

using namespace AccSaber::Utils::Constants;
using namespace rapidjson;

template<class T>
std::optional<T> GetValue(rapidjson::GenericObject<false, rapidjson::Value>& object, std::string name){
    auto itr = object.FindMember(name);
    if (itr != object.MemberEnd() && !itr->value.IsNull()) return std::optional<T>(itr->value.Get<T>());
    else return {};
}

namespace AccSaber::Downloaders{
    void DownloadPlayerInfoAsync(std::string playerId, std::string category, std::function<void(std::optional<Models::AccSaberUserModel>)> callback){
        Utils::SendWebRequest(API_URL + PLAYERS_ENDPOINT + playerId + category, [category, callback](std::string response){
            DEBUG("this is a response from category: {}", category);
            Document document;
            document.Parse(response.c_str());
            if (document.HasMember("errorCode") || document.Empty()) return callback({});            
            auto playerData = document.GetObject();
            auto playerInfo = Models::AccSaberUserModel(
                GetValue<int>(playerData, "rank").value_or(-1),
                GetValue<std::string>(playerData, "playerId").value_or("-1"),
                GetValue<std::string>(playerData, "playerName").value_or(""),
                GetValue<std::string>(playerData, "hmd").value_or("Unknown"),
                GetValue<float>(playerData, "averageAcc").value_or(-1.0f),
                GetValue<float>(playerData, "ap").value_or(-1.0f),
                GetValue<float>(playerData, "averageApPerMap").value_or(-1.0f),
                GetValue<int>(playerData, "rankedPlays").value_or(-1),
                GetValue<bool>(playerData, "accChamp").value_or(false)
            );
            callback(playerInfo);
        });
    }
}