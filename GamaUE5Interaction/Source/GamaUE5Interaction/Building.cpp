// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set default values for attributes
	id = 0;
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


int ABuilding::GetType() const
{
	return type;
}

int32 ABuilding::GetID() const
{
	return id;
}

void ABuilding::SetID(int32 ID)
{
	id = ID;
}
