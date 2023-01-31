// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaActions.h"
#include "GamaClient.h"
#include <iostream>
#include "ExpParameter.h"
#include "GamaActionsMessageHandler.h"


// Sets default values
AGamaActions::AGamaActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	message_handler = new GamaActionsMessageHandler();
	client = new GamaClient("localhost", 6868, message_handler);
}

// Called when the game starts or when spawned
void AGamaActions::BeginPlay()
{
	Super::BeginPlay();
	client -> connect();
}

// Called every frame
void AGamaActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (first && message_handler -> GetSocketId() > 0)
	{
		FString current_path = FPaths::ProjectDir();
		FString url = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead( *(current_path + "../MobilityModel/models/Grid Model.gaml"));
		FString model = "grid_model";
		client -> load(message_handler -> GetSocketId(), url, model);
		first = false;
	}

	// play command
	if(!played && message_handler -> GetExpId() > 0)
	{
		client -> play(message_handler -> GetSocketId(), message_handler -> GetExpId());
		played = true;
	}

	// step/stepBack command
	// expression command
	// if (message_handler -> GetExpId() > 0)
	// {
	// 	client -> step(message_handler -> GetSocketId(), message_handler -> GetExpId(), 2, false);
	// 	client -> expression(message_handler -> GetSocketId(), message_handler -> GetExpId(), "length(prey)");
	// 	// client -> stepBack(message_handler -> GetSocketId(), message_handler -> GetExpId(), 1, false);
	// 	// client -> expression(message_handler -> GetSocketId(), message_handler -> GetExpId(), "length(predator)");
	// }
}

