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
	/*house_ids = {};
	empty_ids = {};
	office_ids = {};
	people_ids = {};

	scaling_factor = 500;
	x_offset = -30000;
	y_offset = -50000;

	houses = {};
	empty_buildings = {};
	offices = {};
	peoples = {};*/

	map = new GamaMap();
}

//bool AObjectHandler::id_found(int32 ID, TArray<int32> ids)
//{
//	return ids.Find(ID) >= 0;
//}
//
//TArray<int32> AObjectHandler::GetHouseIds()
//{
//	return house_ids;
//}
//
//TArray<int32> AObjectHandler::GetEmptyIds()
//{
//	return empty_ids;
//}
//
//TArray<int32> AObjectHandler::GetOfficeIds()
//{
//	return office_ids;
//}
//
//TArray<int32> AObjectHandler::GetPeopleIds()
//{
//	return people_ids;
//}
//
//TArray<AHouse*> AObjectHandler::GetHouses()
//{
//	return houses;
//}
//
//TArray<AEmptyBuilding*> AObjectHandler::GetEmptyBuildings()
//{
//	return empty_buildings;
//}
//
//TArray<AOffice*> AObjectHandler::GetOffices()
//{
//	return offices;
//}
//
//TArray<APeople*> AObjectHandler::GetPeoples()
//{
//	return peoples;
//}

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


	//// destroy old house
	//auto tmp_houses = houses;
	//for (int32 i = 0; i < tmp_houses.Num(); i++)
	//{
	//	bool present = false;
	//	for (int32 j = 0; j < Info->Num(); j++)
	//	{
	//		TSharedPtr<FJsonObject> obj = (*Info)[j]->AsObject();
	//		FString type = obj->GetStringField("type");
	//		int32 ID = obj->GetIntegerField("id");
	//		if (type == "house" && ID == tmp_houses[i]->GetID())
	//		{
	//			present = true;
	//			break;
	//		}
	//	}
	//	if (!present)
	//	{
	//		DestroyBuilding("house", tmp_houses[i]->GetID(), CurrentWorld);
	//	}
	//}

	////destroy old empty building
	//auto tmp_empty_buildings = empty_buildings;
	//for (int32 i = 0; i < tmp_empty_buildings.Num(); i++)
	//{
	//	bool present = false;
	//	for (int32 j = 0; j < Info->Num(); j++)
	//	{
	//		TSharedPtr<FJsonObject> obj = (*Info)[j]->AsObject();
	//		FString type = obj->GetStringField("type");
	//		int32 ID = obj->GetIntegerField("id");
	//		if (type == "empty" && ID == tmp_empty_buildings[i]->GetID())
	//		{
	//			present = true;
	//			break;
	//		}
	//	}
	//	if (!present)
	//	{
	//		DestroyBuilding("empty", tmp_empty_buildings[i]->GetID(), CurrentWorld);
	//	}
	//}

	////destroy old offices
	//auto tmp_offices = offices;
	//for (int32 i = 0; i < tmp_offices.Num(); i++)
	//{
	//	bool present = false;
	//	for (int32 j = 0; j < Info->Num(); j++)
	//	{
	//		TSharedPtr<FJsonObject> obj = (*Info)[j]->AsObject();
	//		FString type = obj->GetStringField("type");
	//		int32 ID = obj->GetIntegerField("id");
	//		if (type == "office" && ID == tmp_offices[i]->GetID())
	//		{
	//			present = true;
	//			break;
	//		}
	//	}
	//	if (!present)
	//	{
	//		DestroyBuilding("office", tmp_offices[i]->GetID(), CurrentWorld);
	//	}
	//}

	//// add new buildings
	//for (int32 i = 0; i < Info->Num(); i++)
	//{
	//	TSharedPtr<FJsonObject> obj = (*Info)[i]->AsObject();
	//	if (obj != NULL)
	//	{
	//		int32 ID = obj->GetIntegerField("id");

	//		FString type = obj->GetStringField("type");
	//		const TSharedPtr<FJsonObject>* Location;
	//		if (obj->TryGetObjectField("location", Location))
	//		{
	//			double x = x_offset + (*Location)->GetNumberField("x") * scaling_factor;
	//			double y = y_offset + (*Location)->GetNumberField("y") * scaling_factor;
	//			const FVector* Loc = new FVector(x, y, 0);

	//			if (type == "house" && !id_found(ID, house_ids))
	//			{
	//				AHouse* house = (AHouse*)CurrentWorld->SpawnActor(AHouse::StaticClass(), Loc);
	//				if (house != NULL)
	//				{
	//					house->SetID(ID);
	//					houses.Add(house);
	//					house_ids.Add(ID);
	//				}
	//			}
	//			if (type == "empty" && !id_found(ID, empty_ids))
	//			{
	//				AEmptyBuilding* empty = (AEmptyBuilding*)CurrentWorld->SpawnActor(AEmptyBuilding::StaticClass(), Loc);
	//				if (empty != NULL)
	//				{
	//					empty->SetID(ID);
	//					empty_buildings.Add(empty);
	//					empty_ids.Add(ID);
	//				}
	//			}
	//			if (type == "office" && !id_found(ID, office_ids))
	//			{
	//				AOffice* office = (AOffice*)CurrentWorld->SpawnActor(AOffice::StaticClass(), Loc);
	//				if (office != NULL)
	//				{
	//					office->SetID(ID);
	//					offices.Add(office);
	//					office_ids.Add(ID);
	//				}
	//			}
	//		}
	//	}
	//}
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
			map->RemovePeople(id);
		}
	}


	//// change people's location
	//auto tmp_peoples = peoples;
	//for (int32 i = 0; i < tmp_peoples.Num(); i++)
	//{
	//	bool present = false;
	//	APeople* people = tmp_peoples[i];
	//	for (int32 j = 0; j < Info->Num(); j++)
	//	{
	//		TSharedPtr<FJsonObject> obj = (*Info)[j]->AsObject();
	//		int32 ID = obj->GetIntegerField("id");
	//		if (ID == tmp_peoples[i]->GetID())
	//		{
	//			const TSharedPtr<FJsonObject>* Location;
	//			if (obj->TryGetObjectField("location", Location))
	//			{
	//				double x = x_offset + (*Location)->GetNumberField("x") * scaling_factor;
	//				double y = y_offset + (*Location)->GetNumberField("y") * scaling_factor;
	//				//const FVector* Loc = new FVector(x, y, 0.0);
	//				double heading = obj->GetNumberField("heading");
	//				tmp_peoples[i]->SetPosition(x, y, heading);
	//				
	//				present = true;
	//				break;
	//			}
	//		}
	//	}
	//	if (!present)
	//	{
	//		DestroyPeople(tmp_peoples[i]->GetID(), CurrentWorld);
	//	}
	//}

	//// add new people
	//for (int32 i = 0; i < Info->Num(); i++)
	//{
	//	TSharedPtr<FJsonObject> obj = (*Info)[i]->AsObject();
	//	if (obj != NULL)
	//	{
	//		int32 ID = obj->GetIntegerField("id");
	//		const TSharedPtr<FJsonObject>* Location;
	//		if (obj->TryGetObjectField("location", Location) && !id_found(ID, people_ids))
	//		{
	//			double x = x_offset + (*Location)->GetNumberField("x") * scaling_factor;
	//			double y = y_offset + (*Location)->GetNumberField("y") * scaling_factor;
	//			double heading = obj->GetNumberField("heading");
	//			APeople* people = (APeople*)CurrentWorld->SpawnActor(APeople::StaticClass());
	//			if (people != nullptr)
	//			{
	//				people->Init(ID, x, y, heading);
	//				peoples.Add(people);
	//				people_ids.Add(ID);
	//			}
	//		}
	//	}
	//}
}


//
//void AObjectHandler::DestroyBuilding(FString type, int32 ID, UWorld* CurrentWorld)
//{
//	if (type == "house")
//	{
//		auto house = houses.FindByPredicate([&](AHouse* item) {return item->GetID() == ID; });
//		if (house != NULL)
//		{
//			bool destroyed_house = CurrentWorld->DestroyActor(*house);
//			if (destroyed_house)
//			{
//				RemoveHouse(*house);
//			}
//		}
//	
//	}
//	if (type == "empty")
//	{
//		auto empty = empty_buildings.FindByPredicate([&](AEmptyBuilding* item) {return item->GetID() == ID; });
//		if (empty != NULL)
//		{
//			bool destroyed_empty = CurrentWorld->DestroyActor(*empty);
//			if (destroyed_empty)
//			{
//				RemoveEmpty(*empty);
//			}
//		}
//		
//	}
//	if (type == "office")
//	{
//		auto office = offices.FindByPredicate([&](AOffice* item) {return item->GetID() == ID; });
//		if (office != NULL)
//		{
//			bool destroyed_office = CurrentWorld->DestroyActor(*office);
//			if (destroyed_office)
//			{
//				RemoveOffice(*office);
//			}
//		}
//	}
//}
//
//void AObjectHandler::RemoveHouse(AHouse* house)
//{
//	int32 ID = house->GetID();
//	house_ids.Remove(ID);
//	houses.RemoveSingle(house);
//	//AGamaActions::SendChange("house", ID);
//}
//
//void AObjectHandler::RemoveEmpty(AEmptyBuilding* empty)
//{
//	int32 ID = empty->GetID();
//	empty_ids.Remove(empty->GetID());
//	empty_buildings.RemoveSingle(empty);
//}
//
//void AObjectHandler::RemoveOffice(AOffice* office)
//{
//	int32 ID = office->GetID();
//	office_ids.Remove(office->GetID());
//	offices.RemoveSingle(office);
//}
//
//void AObjectHandler::DestroyPeople(int32 ID, UWorld* CurrentWorld)
//{
//	auto people = peoples.FindByPredicate([&](APeople* item) {return item->GetID() == ID; });
//	if (people != NULL)
//	{
//		bool destroyed_people = CurrentWorld->DestroyActor(*people);
//		if (destroyed_people)
//		{
//			people_ids.Remove((*people)->GetID());
//			peoples.RemoveSingle(*people);
//		}
//	}
//}

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

