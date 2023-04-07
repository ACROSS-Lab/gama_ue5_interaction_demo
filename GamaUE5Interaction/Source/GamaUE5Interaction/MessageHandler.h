// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GAMAUE5INTERACTION_API MessageHandler
{
protected:
	int32 exp_id;
	int64 socket_id;
public:
	// Set default simulation parameters
	MessageHandler();

	// Get simulation parameters
	int32 GetExpId() const;
	int64 GetSocketId() const;

	// Handle commands returned by Json messages
	void HandleCommand(TSharedPtr<FJsonObject> MyJson);
	virtual void HandleConnectionSuccessful(TSharedPtr<FJsonObject> MyJson) = 0;
	virtual void HandleCommandExecutedSuccessfully(TSharedPtr<FJsonObject> MyJson) = 0;

	virtual ~MessageHandler();
};
