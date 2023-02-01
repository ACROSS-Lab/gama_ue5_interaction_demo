// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "ObjectHandler.h"

/**
 * 
 */
class GAMAUE5INTERACTION_API BuildingHandler: public ObjectHandler
{
public:
	BuildingHandler();
	void HandleBuilding(TSharedPtr<FJsonObject> Info);
	~BuildingHandler();
};
