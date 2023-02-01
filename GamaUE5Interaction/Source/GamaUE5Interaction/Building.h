// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

UCLASS()
class GAMAUE5INTERACTION_API ABuilding : public AActor
{
	GENERATED_BODY()

protected:
	int32 id;
	int32 x_position;
	int32 y_position;

public:	
	// Sets default values for this actor's properties
	ABuilding();

	// Sets values for building's properties
	ABuilding(int32 ID, int32 x, int32 y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// get and set building parameters
	virtual int32 GetID();
	virtual void SetID(int32 ID);
	virtual int32 GetX();
	virtual int32 GetY();
	virtual void SetPosition(int32 x, int32 y);
};
