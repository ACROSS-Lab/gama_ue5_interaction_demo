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
	
	TArray<int32> Get_House_Ids();
	TArray<int32> Get_Empty_Ids();
	TArray<int32> Get_Office_Ids();
	TArray<int32> Get_People_Ids();
	
	void HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld);
	void HandleBuilding(const TArray<TSharedPtr<FJsonValue>>*&Info, UWorld* CurrentWorld);
	void HandlePeople(const TArray<TSharedPtr<FJsonValue>>*&Info, UWorld* CurrentWorld);
	~ObjectHandler();
};
