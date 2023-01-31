// Fill out your copyright notice in the Description page of Project Settings.


#include "People.h"

// Sets default values
APeople::APeople()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APeople::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APeople::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APeople::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

