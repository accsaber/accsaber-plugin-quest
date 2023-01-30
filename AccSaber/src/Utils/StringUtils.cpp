#include "Utils/StringUtils.hpp"

namespace AccSaber::Utils{

    std::string toLower(std::string s){
        for (char& c : s) c = std::tolower(c);
        return s;
    }

    std::string TimeStampToDateString(std::string date){
        std::wistringstream ss{std::wstring(date.begin(),date.end())};
        std::tm dt;
        ss >> std::get_time(&dt, AccSaberSongAPITimeFormat.c_str());
        char dateArray[100];
        strftime(dateArray, 50, "%d.%m.%Y %H.%M.%S", &dt);
        return std::string(dateArray);
    }

    std::vector<std::string> split(const std::string& s, char delim)
    {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim)) elems.push_back(item);
        return elems;
    }
}