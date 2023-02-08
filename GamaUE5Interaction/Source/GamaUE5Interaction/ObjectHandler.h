// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "House.h"
#include "EmptyBuilding.h"
#include "Office.h"
#include "People.h"
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

	TArray<AHouse*> houses;
	TArray<AEmptyBuilding*> empty_buildings;
	TArray<AOffice*> offices;
	TArray<APeople*> peoples;
	
	int32 scaling_factor;
	
public:
	ObjectHandler();

	bool id_found(int32 ID, TArray<int32> ids);
	
	TArray<int32> GetHouseIds();
	TArray<int32> GetEmptyIds();
	TArray<int32> GetOfficeIds();
	TArray<int32> GetPeopleIds();

	TArray<AHouse*> GetHouses();
	TArray<AEmptyBuilding*> GetEmptyBuildings();
	TArray<AOffice*> GetOffices();
	TArray<APeople*> GetPeoples();
	
	void HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld);
	void HandleBuilding(const TArray<TSharedPtr<FJsonValue>>*&Info, UWorld* CurrentWorld);
	void HandlePeople(const TArray<TSharedPtr<FJsonValue>>*&Info, UWorld* CurrentWorld);

	void DestroyBuilding(FString type, int32 ID, UWorld* CurrentWorld);
	void DestroyPeople(int32 ID, UWorld* CurrentWorld);
	
	~ObjectHandler();
};
