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
#include "Containers/Array.h"

ObjectHandler::ObjectHandler()
{
	building_ids = {};
	people_ids = {};
}

bool ObjectHandler::id_found(int32 ID, TArray<int32> ids)
{
	return ids.Find(ID) >= 0;
}


void ObjectHandler::HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld)
{
	//const TSharedPtr<FJsonObject>* Info;



	const TArray<TSharedPtr<FJsonValue>>* BuildingInfo;
	const TArray<TSharedPtr<FJsonValue>>* PeopleInfo;

	if (MyJson->TryGetArrayField("building", BuildingInfo))
	{
		HandleBuidling(BuildingInfo, CurrentWorld);
	}

	if (MyJson->TryGetArrayField("people", PeopleInfo))
	{
		HandlePeople(PeopleInfo, CurrentWorld);
	}
}
void ObjectHandler::HandleBuidling(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	for (int i = 0; i < Info->Num(); i++)
	{
		TSharedPtr<FJsonObject> obj = (*Info)[i]->AsObject();
		if (obj != NULL)
		{
			int32 ID = obj->GetIntegerField("id");

			FString type = obj->GetStringField("type");

			const TSharedPtr<FJsonObject>* Location;

			if (obj->TryGetObjectField("location", Location) && !id_found(ID, building_ids))
			{
				double x = (*Location)->GetNumberField("x");
				double y = (*Location)->GetNumberField("y");

				const FVector* Loc = new FVector(x, y, 10.0);

				if (type == "house")
				{
					//const FVector* Loc = new FVector(x,y, 10.0);
					AHouse* house  = (AHouse*) CurrentWorld->SpawnActor(AHouse::StaticClass(), Loc);
					if (house != NULL)
					{
						house->Init(ID, x, y);
						building_ids.Add(ID);
					}
				}
				if (type == "empty")
				{
					AEmptyBuilding* empty = (AEmptyBuilding*) CurrentWorld->SpawnActor(AEmptyBuilding::StaticClass(), Loc);
					if (empty != NULL)
					{
						empty->Init(ID, x, y);
						building_ids.Add(ID);
					}
				}
				if (type == "office")
				{
					AOffice* office = (AOffice*) CurrentWorld->SpawnActor(AOffice::StaticClass(), Loc);
					if (office != NULL)
					{
						office->Init(ID, x, y);
						building_ids.Add(ID);
					}
				}
			}
		}
	}
}

void ObjectHandler::HandlePeople(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	for (int i = 0; i < Info->Num(); i++)
	{
		TSharedPtr<FJsonObject> obj = (*Info)[i]->AsObject();
		if (obj != NULL)
		{
			int32 ID = obj->GetIntegerField("id");

			const TSharedPtr<FJsonObject>* Location;

			if (obj->TryGetObjectField("location", Location) && !id_found(ID, people_ids))
			{
				double x = (*Location)->GetNumberField("x");
				double y = (*Location)->GetNumberField("y");

				const FVector* Loc = new FVector(x,y, 0.0);
				APeople* people = (APeople*)CurrentWorld->SpawnActor(APeople::StaticClass(), Loc);
				if(people != nullptr)
				{
					people->Init(ID, x, y);
					people_ids.Add(ID);
				}
			}
		}
	}
}

ObjectHandler::~ObjectHandler()
{
}
