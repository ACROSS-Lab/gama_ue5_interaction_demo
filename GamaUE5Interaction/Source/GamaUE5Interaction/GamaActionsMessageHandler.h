// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageHandler.h"

/**
 * 
 */
class GAMAUE5INTERACTION_API GamaActionsMessageHandler: public MessageHandler
{

protected:
	bool playing;
	
public:
	GamaActionsMessageHandler();

	// Handle different types of messages
	void HandleConnectionSuccessful(TSharedPtr<FJsonObject> MyJson);
	void HandleCommandExecutedSuccessfully(TSharedPtr<FJsonObject> MyJson);

	bool IsPlaying();
	
	~GamaActionsMessageHandler();
};
