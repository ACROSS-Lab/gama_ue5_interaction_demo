// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "People.generated.h"

UCLASS(BlueprintType)
class GAMAUE5INTERACTION_API APeople : public ACharacter
{
	GENERATED_BODY()

private:
	// Basic properties
	int32 id;
	FVector position;
	FRotator heading;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh; //Graphical representation

public:
	// Sets default values for this character's properties
	APeople();

	// Sets customized values for this character's properties
	void Init(int32 ID, float x, float y, float heading);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// get and set people parameters
	int32 GetID();
	void SetID(int32 ID);
	int32 GetX();
	int32 GetY();
	void SetPosition(float x, float y, float heading);
};
