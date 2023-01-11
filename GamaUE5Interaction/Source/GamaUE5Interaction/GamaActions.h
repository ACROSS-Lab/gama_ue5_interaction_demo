// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamaClient.h"
#include "MessageHandler.h"
#include "ExpParameter.h"
#include "GamaActions.generated.h"

UCLASS()
class GAMAUE5INTERACTION_API AGamaActions : public AActor
{
	GENERATED_BODY()
	GamaClient* client;
	bool first = true;
	bool played = false;
	GamaActionsMessageHandler* message_handler;
	
public:	
	// Sets default values for this actor's properties
	AGamaActions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
