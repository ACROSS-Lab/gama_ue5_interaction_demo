// Fill out your copyright notice in the Description page of Project Settings.


#include "PeopleHandler.h"
#include "People.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

PeopleHandler::PeopleHandler()
{
}

void PeopleHandler::HandlePeople(TSharedPtr<FJsonObject> Info)
{
	int32 ID = Info->GetIntegerField("id");

	FString type = Info->GetStringField("type");

	const TSharedPtr<FJsonObject>* Location;

	if (Info->TryGetObjectField("location", Location))
	{
		int32 x = (*Location)->GetIntegerField("x");
		int32 y = (*Location)->GetIntegerField("y");

		APeople people(ID, x, y);
	}
}

PeopleHandler::~PeopleHandler()
{
}
