// Fill out your copyright notice in the Description page of Project Settings.


#include "House.h"

// Sets default values
AHouse::AHouse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set default values for attributes
	id = 0;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;

	type = "house";

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	
	// Load the house building
	UStaticMesh* buildingMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/StaticMesh_office_bis.StaticMesh_office_bis'")).Object;
	
	StaticMesh->SetStaticMesh(buildingMesh);
	StaticMesh->SetMobility(EComponentMobility::Static);
	

	//auto Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//UStaticMesh* mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'")).Object;
	//Mesh->SetStaticMesh(mesh);
	//Mesh->AttachedParent = StaticMesh;
	
	RootComponent = StaticMesh;

}

void AHouse::Init(int32 ID, float x, float y)
{


	// Set values for attributes
	id = ID;
	position.X = x;
	position.Y = y;
	position.Z = 0;

	SetActorLocation(position);
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

