// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageHandler.h"
#include "jsoncpp/json/value.h"
#include "jsoncpp/json/json.h"

/**
 * 
 */
class GAMAUE5INTERACTION_API GamaActionsMessageHandler: public MessageHandler
{
public:
	GamaActionsMessageHandler();
	void HandleConnectionSuccessful(TSharedPtr<FJsonObject> MyJson);
	void HandleCommandExecutedSuccessfully(TSharedPtr<FJsonObject> MyJson);
	~GamaActionsMessageHandler();
};
