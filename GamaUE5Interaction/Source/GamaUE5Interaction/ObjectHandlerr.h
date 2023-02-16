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
#include "ObjectHandlerr.generated.h"

UCLASS(BlueprintType)
class GAMAUE5INTERACTION_API AObjectHandlerr : public AActor
{
	GENERATED_BODY()
	
private:
	//UPROPERTY(BlueprintReadOnly)
	TArray<int32> house_ids;
	//UPROPERTY(BlueprintReadOnly)
	TArray<int32> empty_ids;
	//UPROPERTY(BlueprintReadOnly)
	TArray<int32> office_ids;
	//UPROPERTY(BlueprintReadOnly)
	TArray<int32> people_ids;

	//UPROPERTY(BlueprintReadOnly)
	TArray<AHouse*> houses;
	//UPROPERTY(BlueprintReadOnly)
	TArray<AEmptyBuilding*> empty_buildings;
	//UPROPERTY(BlueprintReadOnly)
	TArray<AOffice*> offices;
	//UPROPERTY(BlueprintReadOnly)
	TArray<APeople*> peoples;

	int32 scaling_factor;

	UStaticMeshComponent* StaticMesh;

public:	
	// Sets default values for this actor's properties
	AObjectHandlerr();

	bool id_found(int32 ID, TArray<int32> ids);

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetHouseIds();
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetEmptyIds();
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetOfficeIds();
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetPeopleIds();

	UFUNCTION(BlueprintCallable)
	TArray<AHouse*> GetHouses();
	UFUNCTION(BlueprintCallable)
	TArray<AEmptyBuilding*> GetEmptyBuildings();
	UFUNCTION(BlueprintCallable)
	TArray<AOffice*> GetOffices();
	UFUNCTION(BlueprintCallable)
	TArray<APeople*> GetPeoples();

	void HandleObject(TSharedPtr<FJsonObject> MyJson, UWorld* CurrentWorld);
	void HandleBuilding(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld);
	void HandlePeople(const TArray<TSharedPtr<FJsonValue>>*& Info, UWorld* CurrentWorld);
	void DestroyBuilding(FString type, int32 ID, UWorld* CurrentWorld);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveHouse(AHouse* house);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveEmpty(AEmptyBuilding* empty);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveOffice(AOffice* office);

	void DestroyPeople(int32 ID, UWorld* CurrentWorld);

	~AObjectHandlerr();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
