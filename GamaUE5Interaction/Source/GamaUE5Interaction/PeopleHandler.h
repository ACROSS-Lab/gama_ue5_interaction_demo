// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "ObjectHandler.h"

/**
 * 
 */
class GAMAUE5INTERACTION_API PeopleHandler: public ObjectHandler
{
public:
	PeopleHandler();
	void HandlePeople(TSharedPtr<FJsonObject> Info);
	~PeopleHandler();
};
