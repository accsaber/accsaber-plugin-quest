#pragma once

#include "System/Action_1.hpp"
#include "custom_types/delegate.hpp"

#define DLCompletedDeleg(Type, Func) custom_types::MakeDelegate<Type>(classof(Type), static_cast<std::function<void()>>(Func)) \

typedef System::Action_1<UnityEngine::AsyncOperation*>* DownloadCompletedDelegate;

void WebUtils::SendWebRequest(std::string URL, function_ptr_t<void, std::string> callback) {
    auto request = UnityEngine::Networking::UnityWebRequest::Get(URL);
    request->SetRequestHeader("User-Agent", std::string(ID) + " " + VERSION);
    request->SendWebRequest()->add_completed(DLCompletedDeleg(DownloadCompletedDelegate, [=](){
        callback(request->get_downloadHandler()->GetText());
    }));
}