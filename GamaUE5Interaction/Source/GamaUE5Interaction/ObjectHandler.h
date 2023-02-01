// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * 
 */
class GAMAUE5INTERACTION_API ObjectHandler
{
public:
	ObjectHandler();
	void HandleObject(TSharedPtr<FJsonObject> MyJson);
	virtual void HandleBuidling(TSharedPtr<FJsonObject> Info) = 0;
	virtual void HandlePeople(TSharedPtr<FJsonObject> Info) = 0;
	~ObjectHandler();
};
