// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectHandler.h"
#include "Building.h"
#include "House.h"
#include "EmptyBuilding.h"
#include "Office.h"
#include "People.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Containers/Array.h"
#include "Engine/World.h"
#include "GamaActions.h"

// Sets default values
AObjectHandler::AObjectHandler()
{
	map = new GamaMap();
}


void AObjectHandler::HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld)
{
	const TArray<TSharedPtr<FJsonValue>>* BuildingInfo;
	const TArray<TSharedPtr<FJsonValue>>* PeopleInfo;

	if (MyJson->TryGetArrayField("building", BuildingInfo))
	{
		HandleBuilding(BuildingInfo, CurrentWorld);
	}

	if (MyJson->TryGetArrayField("people", PeopleInfo))
	{
		HandlePeople(PeopleInfo, CurrentWorld);
	}
}

void AObjectHandler::HandleBuilding(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	//we update the visibility of buildings
	for (int32 i = 0; i < Info->Num(); i++)
	{
		int type = (*Info)[i]->AsNumber();
		map->SetBuildingVisible((ABuilding::BuildingTypes)type, i);
	}

}

void AObjectHandler::HandlePeople(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	TSet<int> ids;
	//Start by updating and/or creating people in the simulation
	for (int i = 0; i < Info->Num(); i++)
	{
		TArray<TSharedPtr<FJsonValue>> obj = (*Info)[i]->AsArray();
		int32 id		= obj[0]->AsNumber();
		int32 x			= obj[1]->AsNumber();
		int32 y			= obj[2]->AsNumber();
		int32 heading	= obj[3]->AsNumber();
		map->InitOrUpdatePeople(id, x, y, heading, CurrentWorld);
		ids.Add(id);
		
	}
	//Remove absent people
	//TArray<int> old_people_ids = map->GetPeopleIds(); // we explicitly make a copy
	for (int id : map->GetPeopleIds()) {
		if (!ids.Contains(id)) {
			map->RemovePeople(id, CurrentWorld);
		}
	}

}



AObjectHandler::~AObjectHandler()
{
	delete map;
}

// Called when the game starts or when spawned
void AObjectHandler::BeginPlay()
{
	Super::BeginPlay();
	map->Init(GetWorld());
	
}

void AObjectHandler::EndPlay(const EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);
}

// Called every frame
void AObjectHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

