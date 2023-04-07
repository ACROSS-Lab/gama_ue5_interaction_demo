// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaActionsMessageHandler.h"

GamaActionsMessageHandler::GamaActionsMessageHandler()
{
    // Set to true if you are already running an experiment (and don't need gama server to load/play it)
    playing = true; 
}

void GamaActionsMessageHandler::HandleConnectionSuccessful(TSharedPtr<FJsonObject> MyJson)
{
    if(MyJson -> TryGetNumberField("content", socket_id))
    {
        socket_id = MyJson -> GetIntegerField("content");
    }
}

void GamaActionsMessageHandler::HandleCommandExecutedSuccessfully(TSharedPtr<FJsonObject> MyJson)
{
    //const TSharedPtr<FJsonObject>* Content;
    int OutNumber;
    
    if (MyJson -> TryGetNumberField("content", OutNumber))
    {
        exp_id = OutNumber;
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

bool GamaActionsMessageHandler::IsPlaying() const
{
    return playing;
}

GamaActionsMessageHandler::~GamaActionsMessageHandler()
{
}
