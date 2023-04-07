// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"



UCLASS(BlueprintType)
class GAMAUE5INTERACTION_API ABuilding : public AActor
{
	GENERATED_BODY()

public:	

	enum BuildingTypes
	{
		Empty,
		House,
		Office
	};




	// Sets default values for this actor's properties
	ABuilding();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//// get and set building parameters
	UFUNCTION(BlueprintCallable, Category = "Get Function")
	int GetType() const;
	
	UFUNCTION(BlueprintCallable, Category = "Get Function")
	virtual int32 GetID() const;

	virtual void SetID(int32 ID);

protected:
	// basic building properties
	int32 id;
	BuildingTypes type;

	UStaticMeshComponent* StaticMesh;

};
