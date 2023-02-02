// Fill out your copyright notice in the Description page of Project Settings.


#include "Office.h"

// Sets default values
AOffice::AOffice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values for attributes
	id = 0;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// Load the sphere
	UStaticMesh* sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	
	StaticMesh->SetStaticMesh(sphereMesh);
	StaticMesh->SetMobility(EComponentMobility::Movable);
	//StaticMesh->SetMaterial()
	RootComponent = StaticMesh;
}

void AOffice::Init(int32 ID, float x, float y)
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
void AOffice::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOffice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

