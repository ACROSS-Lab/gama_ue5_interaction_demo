// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageHandler.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Serialization/JsonSerializer.h"
#include "jsoncpp/json/value.h"
#include "jsoncpp/json/json.h"

MessageHandler::MessageHandler()
{
}

int32 MessageHandler::GetExpId()
{
    return exp_id;
}

int64 MessageHandler::GetSocketId()
{
    return socket_id;
}

void MessageHandler::HandleCommand(TSharedPtr<FJsonObject> MyJson)
{
    // FHttpResponsePtr Response;
    FString type;
    // if (MyJson -> TryGetObjectField("content", Content))
    // {
    //     exp_id = (*Content) -> GetIntegerField("exp_id");
    //     UE_LOG(LogTemp, Display, TEXT("exp_id extracted"));
    //     UE_LOG(LogTemp, Display, TEXT("%s"), *FString(std::to_string(exp_id).c_str()));
    // }
    if(MyJson -> TryGetStringField("type", type))
    {
        type = MyJson -> GetStringField("type");
        if(type == "ConnectionSuccessful")
        {
            HandleConnectionSuccessful(MyJson);
        }
        else if (type == "CommandExecutedSuccessfully")
        {
            HandleCommandExecutedSuccessfully(MyJson);
        }
        else
        {
                //
        }
    }
}

MessageHandler::~MessageHandler()
{
}
