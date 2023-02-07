#pragma once
#include <string>

namespace AccSaber::Utils::Constants{
    static const std::string API_URL = "https://api.accsaber.com/";
    static const std::string DEV_API_URL = "https://api.accsaber-dev.ixsen.de/";
    static const std::string CDN_URL = "https://cdn.accsaber.com/";
    static const std::string COVERS_ENDPOINT = "covers/";
    static const std::string RANKED_ENDPOINT = "ranked-maps";
    static const std::string LEADERBOARDS_ENDPOINT = "map-leaderboards/";
    static const std::string PLAYERS_ENDPOINT = "players/";
    static const std::string PAGINATION_PAGE = "?page=";
    static const std::string PAGINATION_PAGESIZE = "&pageSize=";
    static const std::string CATEGORY_ENDPOINT = "categories";
    static const std::string AROUND_ME = "/around/";
    static const std::string OVERALL = "/overall";
    static const std::string STANDARD = "/standard";
    static const std::string TRUE = "/true";
    static const std::string TECH = "/tech";
}