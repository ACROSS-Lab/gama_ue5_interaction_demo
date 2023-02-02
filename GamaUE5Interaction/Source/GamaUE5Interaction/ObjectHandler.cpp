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

ObjectHandler::ObjectHandler()
{
}


void ObjectHandler::HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld)
{
	//const TSharedPtr<FJsonObject>* Info;



	const TArray<TSharedPtr<FJsonValue>>* BuildingInfo;
	const TArray<TSharedPtr<FJsonValue>>* PeopleInfo;

	if (MyJson->TryGetArrayField("building", BuildingInfo))
	{

		HandleBuidling(BuildingInfo, CurrentWorld);
		delete BuildingInfo;
	}

	if (MyJson->TryGetArrayField("people", PeopleInfo))
	{
		HandlePeople(PeopleInfo, CurrentWorld);
		delete PeopleInfo;
	}

}
void ObjectHandler::HandleBuidling(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	for (int i = 0; i < Info->Num(); i++)
	{
		try
		{
			TSharedPtr<FJsonObject> obj = (*Info)[i]->AsObject();
			if (obj != NULL)
			{
				int32 ID = obj->GetIntegerField("id");

				FString type = obj->GetStringField("type");

				const TSharedPtr<FJsonObject>* Location;

				if (obj->TryGetObjectField("location", Location))
				{
					int32 x = (*Location)->GetNumberField("x");
					int32 y = (*Location)->GetNumberField("y");

					if (type == "house")
					{
						AHouse* house  = (AHouse*) CurrentWorld->SpawnActor(AHouse::StaticClass());
						house->Init(ID, x, y);

					}
					if (type == "empty")
					{
						AEmptyBuilding* empty = (AEmptyBuilding*) CurrentWorld->SpawnActor(AEmptyBuilding::StaticClass());
						empty->Init(ID, x, y);
					}
					if (type == "office")
					{
						AOffice* office = (AOffice*) CurrentWorld->SpawnActor(AOffice::StaticClass());
						office->Init(ID, x, y);
					}
				}
			}
		}
		catch (...)
		{

		}
	}
}

void ObjectHandler::HandlePeople(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	for (int i = 0; i < Info->Num(); i++)
	{
		try
		{
			TSharedPtr<FJsonObject> obj = (*Info)[i]->AsObject();
			if (obj != NULL)
			{
				int32 ID = obj->GetIntegerField("id");

				FString type = obj->GetStringField("type");

				const TSharedPtr<FJsonObject>* Location;

				if (obj->TryGetObjectField("location", Location))
				{
					int32 x = (*Location)->GetNumberField("x");
					int32 y = (*Location)->GetNumberField("y");

					APeople* people = (APeople*)CurrentWorld->SpawnActor(APeople::StaticClass());
					people->Init(ID, x, y);
				}
			}
		}
		catch (...)
		{

		}
	}
}

ObjectHandler::~ObjectHandler()
{
}
