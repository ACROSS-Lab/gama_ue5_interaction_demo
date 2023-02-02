// Fill out your copyright notice in the Description page of Project Settings.


#include "EmptyBuilding.h"

// Sets default values
AEmptyBuilding::AEmptyBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values for attributes
	id = 0;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;

	SetActorLocation(position);
}

 void AEmptyBuilding::Init(int32 ID, float x, float y)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set values for attributes
	id = ID;
	position.X = x;
	position.Y = y;
	position.Z = 0;

	SetActorLocation(position);
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

