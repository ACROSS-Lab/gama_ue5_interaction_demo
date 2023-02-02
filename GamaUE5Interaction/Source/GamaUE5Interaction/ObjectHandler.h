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
	void HandleBuidling(const TArray<TSharedPtr<FJsonValue>>*&Info);
	void HandlePeople(const TArray<TSharedPtr<FJsonValue>>*&Info);
	~ObjectHandler();
};
