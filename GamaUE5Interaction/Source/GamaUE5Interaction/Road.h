// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Road.generated.h"

UCLASS()
class GAMAUE5INTERACTION_API ARoad : public AActor
{
	GENERATED_BODY()

private:
	int32 x_position;
	int32 y_position;
	int32 length;

public:	
	// Sets default values for this actor's properties
	ARoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
