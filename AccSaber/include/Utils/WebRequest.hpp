#pragma once
#include <string>
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestAsyncOperation.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/AsyncOperation.hpp"
#include "System/Action_1.hpp"
#include "custom-types/shared/delegate.hpp"

typedef System::Action_1<UnityEngine::AsyncOperation*>* DLFinish;

#define DLCompletedDeleg(Func) custom_types::MakeDelegate<DLFinish>(classof(DLFinish), static_cast<std::function<void(UnityEngine::AsyncOperation*)>>(Func)) \

namespace AccSaber::Utils{
    void SendWebRequest(std::string URL, std::function<void(std::string)> callback);
}