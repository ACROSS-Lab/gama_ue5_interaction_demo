// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dom/JsonObject.h"
#include "House.h"
#include "EmptyBuilding.h"
#include "Office.h"
#include "People.h"
#include "Containers/Array.h"
#include "ObjectHandler.generated.h"

UCLASS(BlueprintType)
class GAMAUE5INTERACTION_API AObjectHandler : public AActor
{
	GENERATED_BODY()
	
private:
	// keeping buildings and people ids
	TArray<int32> house_ids;
	TArray<int32> empty_ids;
	TArray<int32> office_ids;
	TArray<int32> people_ids;

	// keeping buildings and people
	TArray<AHouse*> houses;
	TArray<AEmptyBuilding*> empty_buildings;
	TArray<AOffice*> offices;
	TArray<APeople*> peoples;

	// scaling for better visibility of buildings
	int32 scaling_factor;

	UStaticMeshComponent* StaticMesh;

public:	
	// Sets default values for this actor's properties
	AObjectHandler();

	bool id_found(int32 ID, TArray<int32> ids);

	TArray<int32> GetHouseIds();
	TArray<int32> GetEmptyIds();
	TArray<int32> GetOfficeIds();
	TArray<int32> GetPeopleIds();

	TArray<AHouse*> GetHouses();
	TArray<AEmptyBuilding*> GetEmptyBuildings();
	TArray<AOffice*> GetOffices();
	TArray<APeople*> GetPeoples();

	// Handle changes in the model
	void HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld);
	void HandleBuilding(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld);
	void HandlePeople(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld);
	void DestroyBuilding(FString type, int32 ID, UWorld* CurrentWorld);

	// Remove building IDs and buildings out of arrays
	UFUNCTION(BlueprintCallable)
	virtual void RemoveHouse(AHouse* house);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveEmpty(AEmptyBuilding* empty);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveOffice(AOffice* office);

	void DestroyPeople(int32 ID, UWorld* CurrentWorld);

	~AObjectHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
