// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaMap.h"

GamaMap::GamaMap()
{
}

void GamaMap::Init(UWorld* CurrentWorld)
{
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {

			const FVector* Loc = new FVector(x * scaling_factor + x_offset, y * scaling_factor + y_offset, 0);
			
			AHouse* house = (AHouse*)CurrentWorld->SpawnActor(AHouse::StaticClass(), Loc);
			if (house != nullptr) {
				house->SetActorHiddenInGame(true);
				house->SetActorEnableCollision(false);
				house->SetID(Houses.Num());
				Houses.Add(house);
			}

			AEmptyBuilding* empty = (AEmptyBuilding*)CurrentWorld->SpawnActor(AEmptyBuilding::StaticClass(), Loc);
			if (empty != nullptr) {
				empty->SetActorHiddenInGame(true);
				empty->SetActorEnableCollision(false);
				empty->SetID(EmptyBuildings.Num());
				EmptyBuildings.Add(empty);
			}

			AOffice* office = (AOffice*)CurrentWorld->SpawnActor(AOffice::StaticClass(), Loc);
			if (office != nullptr) {
				office->SetActorHiddenInGame(true);
				office->SetActorEnableCollision(false);
				office->SetID(Offices.Num());
				Offices.Add(office);
			}
		}
	}

}

void GamaMap::InitOrUpdatePeople(int id, int x, int y, int heading, UWorld* CurrentWorld)
{
	int new_x = x_offset + scaling_factor * x;
	int new_y = y_offset + scaling_factor * y;
	
	if (People.Contains(id)) {
		People[id]->SetPosition(new_x, new_y, heading);
	}
	else {

		const FVector* loc = new FVector(new_x, new_y, 0);
		APeople* people = (APeople*)CurrentWorld->SpawnActor(APeople::StaticClass(), loc,new FRotator(0,heading,0));
		if (people != nullptr)
		{
			people->Init(id, new_x, new_y, heading);
			People.Add(id, people);
		}
	}

}

void GamaMap::RemovePeople(int id)
{
	People.Remove(id);
}

void GamaMap::SetBuildingVisible(ABuilding::BuildingTypes t, int id) const
{
	switch (t) {
	case ABuilding::Empty:

		Houses[id]->SetActorHiddenInGame(true);
		Offices[id]->SetActorHiddenInGame(true);
		Houses[id]->SetActorEnableCollision(false);
		Offices[id]->SetActorEnableCollision(false);
		
		EmptyBuildings[id]->SetActorHiddenInGame(false);
		EmptyBuildings[id]->SetActorEnableCollision(true);
		break;

	case ABuilding::House:
		
		Offices[id]->SetActorHiddenInGame(true);
		EmptyBuildings[id]->SetActorHiddenInGame(true);
		Offices[id]->SetActorEnableCollision(false);
		EmptyBuildings[id]->SetActorEnableCollision(false);

		Houses[id]->SetActorHiddenInGame(false);
		Houses[id]->SetActorEnableCollision(true);
		
		break;

	case ABuilding::Office:
		EmptyBuildings[id]->SetActorHiddenInGame(true);
		Houses[id]->SetActorHiddenInGame(true);
		EmptyBuildings[id]->SetActorEnableCollision(false);
		Houses[id]->SetActorEnableCollision(false);

		Offices[id]->SetActorHiddenInGame(false);
		Offices[id]->SetActorEnableCollision(true);
		
		break;
	}
}

void GamaMap::ToggleBuilding(ABuilding::BuildingTypes t, int id) const
{
	ABuilding::BuildingTypes to_set_visible = ABuilding::Empty;
	switch (t) {
	case ABuilding::Empty:
		to_set_visible = ABuilding::House;
		break;
	case ABuilding::House:
		to_set_visible = ABuilding::Office;
	}
	SetBuildingVisible(to_set_visible, id);
}


GamaMap::~GamaMap()
{
}
