// Fill out your copyright notice in the Description page of Project Settings.


#include "EmptyBuilding.h"

// Sets default values
AEmptyBuilding::AEmptyBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values for attributes
	id = 0;
	x_position = 0;
	y_position = 0;

}

AEmptyBuilding::AEmptyBuilding(int32 ID, int32 x, int32 y)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set values for attributes
	id = ID;
	x_position = x;
	y_position = y;
}

// Called when the game starts or when spawned
void AEmptyBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEmptyBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

