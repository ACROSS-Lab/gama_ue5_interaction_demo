// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamaClient.h"
#include "GamaActionsMessageHandler.h"
#include "Common/TcpSocketBuilder.h"

#include "GamaActions.generated.h"

UCLASS()
class GAMAUE5INTERACTION_API AGamaActions : public AActor
{
	GENERATED_BODY()
	
	// Connection constants
	const FString GAMA_URL		= "localhost";
	const FString GAMA_IP		= "127.0.0.1";
	const int GAMA_SERVER_PORT	= 6868;
	const uint16 TCP_PORT		= 8000;

	//Gama server handling
	GamaClient* client;
	GamaActionsMessageHandler* message_handler;

	// TCP handling
	FSocket* TcpSocket;
	
	// Game logic
	bool first = true;
	bool played = false;
	bool tcp_connected = false;

	
public:	
	// Sets default values for this actor's properties
	AGamaActions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
