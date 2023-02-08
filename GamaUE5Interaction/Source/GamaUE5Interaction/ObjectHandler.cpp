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

	scaling_factor = 10;

	houses = {};
	empty_buildings = {};
	offices = {};
	peoples = {};
}

bool ObjectHandler::id_found(int32 ID, TArray<int32> ids)
{
	return ids.Find(ID) >= 0;
}

TArray<int32> ObjectHandler::GetHouseIds()
{
	return house_ids;
}

TArray<int32> ObjectHandler::GetEmptyIds()
{
	return empty_ids;
}

TArray<int32> ObjectHandler::GetOfficeIds()
{
	return office_ids;
}

TArray<int32> ObjectHandler::GetPeopleIds()
{
	return people_ids;
}

TArray<AHouse*> ObjectHandler::GetHouses()
{
	return houses;
}

TArray<AEmptyBuilding*> ObjectHandler::GetEmptyBuildings()
{
	return empty_buildings;
}

TArray<AOffice*> ObjectHandler::GetOffices()
{
	return offices;
}

TArray<APeople*> ObjectHandler::GetPeoples()
{
	return peoples;
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
				double x = (*Location)->GetNumberField("x") * scaling_factor;
				double y = (*Location)->GetNumberField("y") * scaling_factor;

				const FVector* Loc = new FVector(x, y, 10.0);

				if (type == "house" && !id_found(ID, house_ids))
				{
					//const FVector* Loc = new FVector(x,y, 10.0);
					AHouse* house  = (AHouse*) CurrentWorld->SpawnActor(AHouse::StaticClass(), Loc);
					if (house != NULL)
					{
						house->Init(ID, x, y);
						houses.Add(house);
						house_ids.Add(ID);
					}
				}
				if (type == "empty" && !id_found(ID, empty_ids))
				{
					AEmptyBuilding* empty = (AEmptyBuilding*) CurrentWorld->SpawnActor(AEmptyBuilding::StaticClass(), Loc);
					if (empty != NULL)
					{
						empty->Init(ID, x, y);
						empty_buildings.Add(empty);
						empty_ids.Add(ID);
					}
				}
				if (type == "office" && !id_found(ID, office_ids))
				{
					AOffice* office = (AOffice*) CurrentWorld->SpawnActor(AOffice::StaticClass(), Loc);
					if (office != NULL)
					{
						office->Init(ID, x, y);
						offices.Add(office);
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
				double x = (*Location)->GetNumberField("x") * scaling_factor;
				double y = (*Location)->GetNumberField("y") * scaling_factor;

				const FVector* Loc = new FVector(x,y, 0.0);
				APeople* people = (APeople*)CurrentWorld->SpawnActor(APeople::StaticClass(), Loc);
				if(people != nullptr)
				{
					people->Init(ID, x, y);
					peoples.Add(people);
					people_ids.Add(ID);
				}
			}
		}
	}
}

void ObjectHandler::DestroyBuilding(FString type, int32 ID, UWorld* CurrentWorld)
{
	if(type == "house")
	{
		for(int32 i = 0; i < houses.Num(); i++)
		{
			if(houses[i] -> GetID() == ID)
			{
				bool destroyed_house = CurrentWorld -> DestroyActor(houses[i]);
				if(destroyed_house)
				{
					house_ids.Remove(houses[i] -> GetID());
					houses.Remove(houses[i]);
					break;	
				}
			}
		}
	}
	if(type == "empty")
	{
		for(int32 i = 0; i < empty_buildings.Num(); i++)
		{
			if(empty_buildings[i] -> GetID() == ID)
			{
				bool destroyed_empty = CurrentWorld -> DestroyActor(empty_buildings[i]);
				if(destroyed_empty)
				{
					empty_ids.Remove(empty_buildings[i] -> GetID());
					empty_buildings.Remove(empty_buildings[i]);
					break;
				}
			}
		}
	}
	if(type == "office")
	{
		for(int32 i = 0; i < offices.Num(); i++)
		{
			if(offices[i] -> GetID() == ID)
			{
				bool destroyed_office = CurrentWorld -> DestroyActor(offices[i]);
				if(destroyed_office)
				{
					office_ids.Remove(offices[i] -> GetID());
					offices.Remove(offices[i]);
					break;
				}
			}
		}
	}
}

void ObjectHandler::DestroyPeople(int32 ID, UWorld* CurrentWorld)
{
	for(int32 i = 0; i < peoples.Num(); i++)
	{
		if(peoples[i] -> GetID() == ID)
		{
			bool destroyed_people = CurrentWorld -> DestroyActor(peoples[i]);
			if(destroyed_people)
			{
				people_ids.Remove(peoples[i] -> GetID());
				peoples.Remove(peoples[i]);
				break;
			}
		}
	}
}

ObjectHandler::~ObjectHandler()
{
}
