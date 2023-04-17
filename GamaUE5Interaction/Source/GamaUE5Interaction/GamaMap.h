// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmptyBuilding.h"
#include "Office.h"
#include "House.h"
#include "People.h"
#include "Building.h"



/**
 * 
 */
class GAMAUE5INTERACTION_API GamaMap
{

protected:

	// Items lists
	TArray<AEmptyBuilding*>	EmptyBuildings;
	TArray<AOffice*>		Offices;
	TArray<AHouse*>			Houses;
	TMap<int, APeople*>		People;

	// Transformation parameters to scale to the world coordinates
	const int32 scaling_factor			= 500;
	const int32 building_width_in_gama	= 30;
	const int32 building_height_in_gama = 30;
	const int32 x_offset				= -30000;
	const int32 y_offset				= -50000;


public:


	GamaMap();

	void Init(UWorld* CurrentWorld);

	void InitOrUpdatePeople(int id, int x, int y, int heading, UWorld* CurrentWorld);
	void RemovePeople(int id);
	void SetBuildingVisible(ABuilding::BuildingTypes t, int id) const;

	void ToggleBuilding(ABuilding::BuildingTypes t, int id) const;


	inline APeople* GetPeople(int id) const {
		if (!People.Contains(id)) {
			return nullptr;
		}
		return People[id];
	}
	
	inline TArray<int> GetPeopleIds() const {
		TArray<int> out;
		People.GenerateKeyArray(out);
		return out;
	}

	inline TArray<APeople*> GetPeople() const {
		TArray<APeople*> out;
		People.GenerateValueArray(out);
		return out;
	}


	~GamaMap();
};
