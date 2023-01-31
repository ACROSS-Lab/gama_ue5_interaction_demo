// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaActionsMessageHandler.h"

GamaActionsMessageHandler::GamaActionsMessageHandler()
{
    // Set to true if you are already running an experiment (and don't need gama server to load/play it)
    playing = false; 
}

void GamaActionsMessageHandler::HandleConnectionSuccessful(TSharedPtr<FJsonObject> MyJson)
{
    if(MyJson -> TryGetNumberField("content", socket_id))
    {
        socket_id = MyJson -> GetIntegerField("content");
        // UE_LOG(LogTemp, Display, TEXT("socket_id extracted"));
        // UE_LOG(LogTemp, Display, TEXT("%s"), *FString(std::to_string(socket_id).c_str()));
    }
}

void GamaActionsMessageHandler::HandleCommandExecutedSuccessfully(TSharedPtr<FJsonObject> MyJson)
{
    //const TSharedPtr<FJsonObject>* Content;
    int OutNumber;
    
    if (MyJson -> TryGetNumberField("content", OutNumber))
    {
        exp_id = OutNumber;
        // UE_LOG(LogTemp, Display, TEXT("exp_id extracted"));
        // UE_LOG(LogTemp, Display, TEXT("%s"), *FString(std::to_string(exp_id).c_str()));
    }

    
    const TSharedPtr<FJsonObject>* Command;
    if (MyJson -> TryGetObjectField("command", Command))
    {
        FString CommandName;
        if ((*Command) ->TryGetStringField("type", CommandName) && CommandName.Equals("play") )
        {
            playing = true;
        }
    }
}

bool GamaActionsMessageHandler::IsPlaying()
{
    return playing;
}

GamaActionsMessageHandler::~GamaActionsMessageHandler()
{
}
