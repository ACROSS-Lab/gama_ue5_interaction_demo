// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingHandler.h"
#include "Building.h"
#include "House.h"
#include "EmptyBuilding.h"
#include "Office.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

BuildingHandler::BuildingHandler()
{
}

void BuildingHandler::HandleBuilding(TSharedPtr<FJsonObject> Info)
{	
	int32 ID = Info->GetIntegerField("id");

	FString type = Info->GetStringField("type");

	const TSharedPtr<FJsonObject>* Location;

	if (Info->TryGetObjectField("location", Location))
	{
		int32 x = (*Location)->GetIntegerField("x");
		int32 y = (*Location)->GetIntegerField("y");

		if (type == "house")
		{
			AHouse house = AHouse(ID, x, y);
		}
		if (type == "empty")
		{
			AEmptyBuilding empty = AEmptyBuilding(ID, x, y);
		}
		if (type == "office")
		{
			AOffice office = AOffice(ID, x, y);
		}
	}
}

BuildingHandler::~BuildingHandler()
{
}
