// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ABuilding::SetID(int ID)
{
	id = ID;
}

int32 ABuilding::GetX()
{
	return x_position;
}

int32 ABuilding::GetY()
{
	return y_position;
}

void ABuilding::SetPosition(int x, int y)
{
	x_position = x;
	y_position = y;
}

