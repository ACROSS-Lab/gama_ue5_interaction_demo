// Fill out your copyright notice in the Description page of Project Settings.


#include "EmptyBuilding.h"

// Sets default values
AEmptyBuilding::AEmptyBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

