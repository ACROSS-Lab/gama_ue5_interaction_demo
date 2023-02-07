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
	house_ids = {};
	empty_ids = {};
	office_ids = {};
	people_ids = {};
}

bool ObjectHandler::id_found(int32 ID, TArray<int32> ids)
{
	return ids.Find(ID) >= 0;
}

TArray<int32> ObjectHandler::Get_House_Ids()
{
	return house_ids;
}

TArray<int32> ObjectHandler::Get_Empty_Ids()
{
	return empty_ids;
}

TArray<int32> ObjectHandler::Get_Office_Ids()
{
	return office_ids;
}

TArray<int32> ObjectHandler::Get_People_Ids()
{
	return people_ids;
}


void ObjectHandler::HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld)
{
	//const TSharedPtr<FJsonObject>* Info;



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
void ObjectHandler::HandleBuilding(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	for (int i = 0; i < Info->Num(); i++)
	{
		TSharedPtr<FJsonObject> obj = (*Info)[i]->AsObject();
		if (obj != NULL)
		{
			int32 ID = obj->GetIntegerField("id");

			FString type = obj->GetStringField("type");

			const TSharedPtr<FJsonObject>* Location;

			if (obj->TryGetObjectField("location", Location))
			{
				double x = (*Location)->GetNumberField("x");
				double y = (*Location)->GetNumberField("y");

				const FVector* Loc = new FVector(x, y, 10.0);

				if (type == "house" && !id_found(ID, house_ids))
				{
					//const FVector* Loc = new FVector(x,y, 10.0);
					AHouse* house  = (AHouse*) CurrentWorld->SpawnActor(AHouse::StaticClass(), Loc);
					if (house != NULL)
					{
						house->Init(ID, x, y);
						house_ids.Add(ID);
					}
				}
				if (type == "empty" && !id_found(ID, empty_ids))
				{
					AEmptyBuilding* empty = (AEmptyBuilding*) CurrentWorld->SpawnActor(AEmptyBuilding::StaticClass(), Loc);
					if (empty != NULL)
					{
						empty->Init(ID, x, y);
						empty_ids.Add(ID);
					}
				}
				if (type == "office" && !id_found(ID, office_ids))
				{
					AOffice* office = (AOffice*) CurrentWorld->SpawnActor(AOffice::StaticClass(), Loc);
					if (office != NULL)
					{
						office->Init(ID, x, y);
						office_ids.Add(ID);
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
