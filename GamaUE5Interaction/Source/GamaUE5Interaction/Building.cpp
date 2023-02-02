// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

// Sets default values
ABuilding::ABuilding()
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

ABuilding::ABuilding(int32 ID, float x, float y)
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
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ABuilding::GetID()
{
	return id;
}

void ABuilding::SetID(int32 ID)
{
	id = ID;
}

float ABuilding::GetX()
{
	return position.X;
}

float ABuilding::GetY()
{
	return position.Y;
}

void ABuilding::SetPosition(int32 x, int32 y)
{
	position.X = x;
	position.Y = y;
	SetActorLocation(position);
}

