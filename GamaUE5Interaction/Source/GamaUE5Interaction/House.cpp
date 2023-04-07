// Fill out your copyright notice in the Description page of Project Settings.


#include "House.h"

// Sets default values
AHouse::AHouse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set default values for attributes
	id = -1;

	type = House;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	
	// Load the house building
	UStaticMesh* buildingMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/StaticMesh_office_bis.StaticMesh_office_bis'")).Object;
	
	StaticMesh->SetStaticMesh(buildingMesh);
	StaticMesh->SetMobility(EComponentMobility::Static);
	
	RootComponent = StaticMesh;

}

// Called when the game starts or when spawned
void AHouse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

