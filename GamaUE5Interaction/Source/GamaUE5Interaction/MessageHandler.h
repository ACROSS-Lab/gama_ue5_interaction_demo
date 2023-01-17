// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GAMAUE5INTERACTION_API MessageHandler
{
protected:
	int32 exp_id = 0;
	int64 socket_id = 0;
public:
	MessageHandler();
	int32 GetExpId();
	int64 GetSocketId();
	void HandleCommand(TSharedPtr<FJsonObject> MyJson);
	virtual void HandleConnectionSuccessful(TSharedPtr<FJsonObject> MyJson) = 0;
	virtual void HandleCommandExecutedSuccessfully(TSharedPtr<FJsonObject> MyJson) = 0;
	virtual ~MessageHandler();
};
