// Fill out your copyright notice in the Description page of Project Settings.


#include "EmptyBuilding.h"

// Sets default values
AEmptyBuilding::AEmptyBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set default values for attributes
	id = 0;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;

	type = "empty";

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// Load the sphere
	UStaticMesh* mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'")).Object;
	StaticMesh->SetStaticMesh(mesh);
	StaticMesh->SetWorldScale3D(FVector3d(150, 150, 1));
	StaticMesh->SetMobility(EComponentMobility::Static);
	
	RootComponent = StaticMesh;
}

 void AEmptyBuilding::Init(int32 ID, float x, float y)
{
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

