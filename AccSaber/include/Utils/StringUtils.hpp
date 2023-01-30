#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>

namespace AccSaber::Utils{
    static const std::wstring AccSaberSongAPITimeFormat{L"%Y-%m-%dT%H:%M:%SZ"};
    std::string TimeStampToDateString(std::string date);
    std::string toLower(std::string string);
    std::vector<std::string> split(const std::string& s, char delim);
}