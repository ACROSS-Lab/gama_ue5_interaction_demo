// Fill out your copyright notice in the Description page of Project Settings.


#include "People.h"

// Sets default values
APeople::APeople()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values for attributes
	id = 0;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// Load the car
	UStaticMesh* mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_vehicule.SM_vehicule'")).Object;

	StaticMesh->SetStaticMesh(mesh);
	StaticMesh->SetMobility(EComponentMobility::Movable);
	RootComponent = StaticMesh;
	
}

void APeople::Init(int32 ID, float x, float y, float rotation)
{
	// Set values for attributes
	id = ID;
	position.X = x;
	position.Y = y;
	heading = FRotator(0, rotation,0);

	//not needed anymore
	//SetActorLocationAndRotation(position, heading);
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

int32 APeople::GetID()
{
	return id;
}

void APeople::SetID(int32 ID)
{
	id = ID;
}

int32 APeople::GetX()
{
	return position.X;
}

int32 APeople::GetY()
{
	return position.Y;
}

void APeople::SetPosition(float x, float y, float rotation)
{
	position.X = x;
	position.Y = y;
	this->heading = FRotator(0, rotation, 0);
	SetActorLocationAndRotation(position, heading, true);
}

