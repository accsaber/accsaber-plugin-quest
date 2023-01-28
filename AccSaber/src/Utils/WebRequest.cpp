#include "Utils/WebRequest.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "main.hpp"

namespace AccSaber::Utils{
    void SendWebRequest(std::string URL, std::function<void(std::string)> callback){
        QuestUI::MainThreadScheduler::Schedule([=](){
            auto request = UnityEngine::Networking::UnityWebRequest::Get(URL);
            request->SetRequestHeader("User-Agent", std::string(MOD_ID) + " " + VERSION);
            request->SendWebRequest()->add_completed(DLCompletedDeleg([=](auto* value){
                callback(request->get_downloadHandler()->GetText());
            }));
        });
        
    }
}