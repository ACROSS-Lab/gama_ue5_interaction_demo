// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectHandlerr.h"
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
AObjectHandlerr::AObjectHandlerr()
{
	house_ids = {};
	empty_ids = {};
	office_ids = {};
	people_ids = {};

	scaling_factor = 5;

	houses = {};
	empty_buildings = {};
	offices = {};
	peoples = {};

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// Load the material
	UStaticMesh* sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	StaticMesh->SetStaticMesh(sphereMesh);
	StaticMesh->SetMobility(EComponentMobility::Movable);
	RootComponent = StaticMesh;
}

bool AObjectHandlerr::id_found(int32 ID, TArray<int32> ids)
{
	return ids.Find(ID) >= 0;
}

TArray<int32> AObjectHandlerr::GetHouseIds()
{
	return house_ids;
}

TArray<int32> AObjectHandlerr::GetEmptyIds()
{
	return empty_ids;
}

TArray<int32> AObjectHandlerr::GetOfficeIds()
{
	return office_ids;
}

TArray<int32> AObjectHandlerr::GetPeopleIds()
{
	return people_ids;
}

TArray<AHouse*> AObjectHandlerr::GetHouses()
{
	return houses;
}

TArray<AEmptyBuilding*> AObjectHandlerr::GetEmptyBuildings()
{
	return empty_buildings;
}

TArray<AOffice*> AObjectHandlerr::GetOffices()
{
	return offices;
}

TArray<APeople*> AObjectHandlerr::GetPeoples()
{
	return peoples;
}

void AObjectHandlerr::HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld)
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

void AObjectHandlerr::HandleBuilding(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	// destroy old house
	auto tmp_houses = houses;
	for (int32 i = 0; i < tmp_houses.Num(); i++)
	{
		bool present = false;
		for (int32 j = 0; j < Info->Num(); j++)
		{
			TSharedPtr<FJsonObject> obj = (*Info)[j]->AsObject();
			FString type = obj->GetStringField("type");
			int32 ID = obj->GetIntegerField("id");
			if (type == "house" && ID == tmp_houses[i]->GetID())
			{
				present = true;
				break;
			}
		}
		if (!present)
		{
			DestroyBuilding("house", tmp_houses[i]->GetID(), CurrentWorld);
		}
	}

	//destroy old empty building
	auto tmp_empty_buildings = empty_buildings;
	for (int32 i = 0; i < tmp_empty_buildings.Num(); i++)
	{
		bool present = false;
		for (int32 j = 0; j < Info->Num(); j++)
		{
			TSharedPtr<FJsonObject> obj = (*Info)[j]->AsObject();
			FString type = obj->GetStringField("type");
			int32 ID = obj->GetIntegerField("id");
			if (type == "empty" && ID == tmp_empty_buildings[i]->GetID())
			{
				present = true;
				break;
			}
		}
		if (!present)
		{
			DestroyBuilding("empty", tmp_empty_buildings[i]->GetID(), CurrentWorld);
		}
	}

	//destroy old offices
	auto tmp_offices = offices;
	for (int32 i = 0; i < tmp_offices.Num(); i++)
	{
		bool present = false;
		for (int32 j = 0; j < Info->Num(); j++)
		{
			TSharedPtr<FJsonObject> obj = (*Info)[j]->AsObject();
			FString type = obj->GetStringField("type");
			int32 ID = obj->GetIntegerField("id");
			if (type == "office" && ID == tmp_offices[i]->GetID())
			{
				present = true;
				break;
			}
		}
		if (!present)
		{
			DestroyBuilding("office", tmp_offices[i]->GetID(), CurrentWorld);
		}
	}

	// add new buildings
	for (int32 i = 0; i < Info->Num(); i++)
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

				const FVector* Loc = new FVector(x, y, 0);

				if (type == "house" && !id_found(ID, house_ids))
				{
					AHouse* house = (AHouse*)CurrentWorld->SpawnActor(AHouse::StaticClass(), Loc);
					if (house != NULL)
					{
						house->Init(ID, x, y);
						houses.Add(house);
						house_ids.Add(ID);
					}
				}
				if (type == "empty" && !id_found(ID, empty_ids))
				{
					AEmptyBuilding* empty = (AEmptyBuilding*)CurrentWorld->SpawnActor(AEmptyBuilding::StaticClass(), Loc);
					if (empty != NULL)
					{
						empty->Init(ID, x, y);
						empty_buildings.Add(empty);
						empty_ids.Add(ID);
					}
				}
				if (type == "office" && !id_found(ID, office_ids))
				{
					AOffice* office = (AOffice*)CurrentWorld->SpawnActor(AOffice::StaticClass(), Loc);
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

void AObjectHandlerr::HandlePeople(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld)
{
	// change people's location
	auto tmp_peoples = peoples;
	for (int32 i = 0; i < tmp_peoples.Num(); i++)
	{
		bool present = false;
		APeople* people = tmp_peoples[i];
		for (int32 j = 0; j < Info->Num(); j++)
		{
			TSharedPtr<FJsonObject> obj = (*Info)[j]->AsObject();
			int32 ID = obj->GetIntegerField("id");
			if (ID == tmp_peoples[i]->GetID())
			{
				const TSharedPtr<FJsonObject>* Location;
				if (obj->TryGetObjectField("location", Location))
				{
					double x = (*Location)->GetNumberField("x") * scaling_factor;
					double y = (*Location)->GetNumberField("y") * scaling_factor;
					//const FVector* Loc = new FVector(x, y, 0.0);
					tmp_peoples[i]->SetPosition(x, y);
					present = true;
					break;
				}
			}
		}
		if (!present)
		{
			DestroyPeople(tmp_peoples[i]->GetID(), CurrentWorld);
		}
	}

	// add new people
	for (int32 i = 0; i < Info->Num(); i++)
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

				const FVector* Loc = new FVector(x, y, 0.0);
				APeople* people = (APeople*)CurrentWorld->SpawnActor(APeople::StaticClass(), Loc);
				if (people != nullptr)
				{
					people->Init(ID, x, y);
					peoples.Add(people);
					people_ids.Add(ID);
				}
			}
		}
	}
}

void AObjectHandlerr::DestroyBuilding(FString type, int32 ID, UWorld* CurrentWorld)
{
	if (type == "house")
	{
		auto house = houses.FindByPredicate([&](AHouse* item) {return item->GetID() == ID; });
		if (house != NULL)
		{
			bool destroyed_house = CurrentWorld->DestroyActor(*house);
			if (destroyed_house)
			{
				RemoveHouse(*house);
			}
		}
		/*for(int32 i = 0; i < houses.Num(); i++)
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
		}*/
	}
	if (type == "empty")
	{
		auto empty = empty_buildings.FindByPredicate([&](AEmptyBuilding* item) {return item->GetID() == ID; });
		if (empty != NULL)
		{
			bool destroyed_empty = CurrentWorld->DestroyActor(*empty);
			if (destroyed_empty)
			{
				RemoveEmpty(*empty);
			}
		}
		/*for(int32 i = 0; i < empty_buildings.Num(); i++)
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
		}*/
	}
	if (type == "office")
	{
		auto office = offices.FindByPredicate([&](AOffice* item) {return item->GetID() == ID; });
		if (office != NULL)
		{
			bool destroyed_office = CurrentWorld->DestroyActor(*office);
			if (destroyed_office)
			{
				RemoveOffice(*office);
			}
		}
		/*for(int32 i = 0; i < offices.Num(); i++)
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
		}*/
	}
}

void AObjectHandlerr::RemoveHouse(AHouse* house)
{
	int32 ID = house->GetID();
	house_ids.Remove(ID);
	houses.RemoveSingle(house);
	//AGamaActions::SendChange("house", ID);
}

void AObjectHandlerr::RemoveEmpty(AEmptyBuilding* empty)
{
	int32 ID = empty->GetID();
	empty_ids.Remove(empty->GetID());
	empty_buildings.RemoveSingle(empty);
}

void AObjectHandlerr::RemoveOffice(AOffice* office)
{
	int32 ID = office->GetID();
	office_ids.Remove(office->GetID());
	offices.RemoveSingle(office);
}

void AObjectHandlerr::DestroyPeople(int32 ID, UWorld* CurrentWorld)
{
	auto people = peoples.FindByPredicate([&](APeople* item) {return item->GetID() == ID; });
	if (people != NULL)
	{
		bool destroyed_people = CurrentWorld->DestroyActor(*people);
		if (destroyed_people)
		{
			people_ids.Remove((*people)->GetID());
			peoples.RemoveSingle(*people);
		}
	}
	/*for(int32 i = 0; i < peoples.Num(); i++)
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
	}*/
}

AObjectHandlerr::~AObjectHandlerr()
{
}

// Called when the game starts or when spawned
void AObjectHandlerr::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectHandlerr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

