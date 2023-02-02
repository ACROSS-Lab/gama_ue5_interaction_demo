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

			if (obj->TryGetObjectField("location", Location))
			{
				double x = (*Location)->GetNumberField("x");
				double y = (*Location)->GetNumberField("y");

				if (type == "house")
				{
					const FVector* Loc = new FVector(x,y, 10.0);
					AHouse* house  = (AHouse*) CurrentWorld->SpawnActor(AHouse::StaticClass(),Loc );
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

			if (obj->TryGetObjectField("location", Location))
			{
				double x = (*Location)->GetNumberField("x");
				double y = (*Location)->GetNumberField("y");

				const FVector* Loc = new FVector(x,y, 0.0);
				APeople* people = (APeople*)CurrentWorld->SpawnActor(APeople::StaticClass(), Loc);
				if(people != nullptr)
				{
					people->Init(ID, x, y);
				}
			}
		}
	}
}

ObjectHandler::~ObjectHandler()
{
}
