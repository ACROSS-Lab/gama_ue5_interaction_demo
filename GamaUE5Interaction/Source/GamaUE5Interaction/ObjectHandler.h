// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Containers/Array.h"

/**
 * 
 */
class GAMAUE5INTERACTION_API ObjectHandler
{
private:
	TArray<int32> house_ids;
	TArray<int32> empty_ids;
	TArray<int32> office_ids;
	TArray<int32> people_ids;
public:
	ObjectHandler();

	bool id_found(int32 ID, TArray<int32> ids);

	void HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld);
	void HandleBuidling(const TArray<TSharedPtr<FJsonValue>>*&Info, UWorld* CurrentWorld);
	void HandlePeople(const TArray<TSharedPtr<FJsonValue>>*&Info, UWorld* CurrentWorld);
	~ObjectHandler();
};
