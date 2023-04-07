// Fill out your copyright notice in the Description page of Project Settings.


#include "Office.h"

// Sets default values
AOffice::AOffice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set default values for attributes
	id = -1;

	type = Office;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	
	// Load the building
	UStaticMesh* mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/StaticMesh_office.StaticMesh_office'")).Object;	
	StaticMesh->SetStaticMesh(mesh);
	StaticMesh->SetMobility(EComponentMobility::Static);
	
	RootComponent = StaticMesh;
	
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

