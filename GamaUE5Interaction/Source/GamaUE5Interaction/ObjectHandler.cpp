// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectHandler.h"
#include "Building.h"
#include "House.h"
#include "EmptyBuilding.h"
#include "Office.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

ObjectHandler::ObjectHandler()
{
}

void ObjectHandler::HandleObject(TSharedPtr<FJsonObject> MyJson)
{
	const TSharedPtr<FJsonObject>* Info;
	
	if (MyJson -> TryGetObjectField("building", Info))
	{
		HandleBuidling(*(Info));
	}

	if (MyJson->TryGetObjectField("people", Info))
	{
		HandlePeople(*(Info));
	}

}

ObjectHandler::~ObjectHandler()
{
}
