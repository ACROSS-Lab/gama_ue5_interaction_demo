// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageHandler.h"
#include "Serialization/JsonSerializer.h"

MessageHandler::MessageHandler()
{
    // Set default simulation parameters
    exp_id = 0;
    socket_id = 0;
}

int32 MessageHandler::GetExpId() const
{
    return exp_id;
}

int64 MessageHandler::GetSocketId() const
{
    return socket_id;
}

void MessageHandler::HandleCommand(TSharedPtr<FJsonObject> MyJson)
{
    // FHttpResponsePtr Response;
    FString type;
   
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
    }
}

MessageHandler::~MessageHandler()
{
}
