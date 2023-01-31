// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.h"
#include "EmptyBuilding.generated.h"

UCLASS()
class GAMAUE5INTERACTION_API AEmptyBuilding : public ABuilding
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmptyBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};