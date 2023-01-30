#pragma once
#include "Models/AccSaberUserModel.hpp"
#include <optional>
#include <functional>
#include <string>

namespace AccSaber::Downloaders{
    void DownloadPlayerInfoAsync(std::string playerId, std::function<void(std::optional<Models::AccSaberUserModel>)> callback);
}