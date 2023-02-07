// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaActions.h"
#include "GamaClient.h"
#include "ExpParameter.h"
#include "ObjectHandler.h"
#include "GamaActionsMessageHandler.h"
#include "Common/TcpSocketBuilder.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"


// Sets default values
AGamaActions::AGamaActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGamaActions::BeginPlay()
{
	Super::BeginPlay();

	// we move the initializations here because the actors are constructed multiple times
	message_handler = new GamaActionsMessageHandler();
	client = new GamaClient(GAMA_URL, GAMA_SERVER_PORT, message_handler);
	client -> connect();
	ObjHandler = new ObjectHandler();
}

// Called every frame
void AGamaActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (first && message_handler -> GetSocketId() > 0)
	{
		FString current_path = FPaths::ProjectDir();
		FString url = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead( *(current_path + "../MobilityModel/models/Grid Model.gaml"));
		FString model = "grid_model";
		client -> load(message_handler -> GetSocketId(), url, model);
		first = false;
	}

	// play command
	if(!played && message_handler -> GetExpId() > 0)
	{
		client -> play(message_handler -> GetSocketId(), message_handler -> GetExpId(), true);
		played = true;
	}

	// Connects to TCP server
	if (!tcp_connected && message_handler -> IsPlaying())
	{
		FIPv4Address IPAddress;
		FIPv4Address::Parse(GAMA_IP, IPAddress);
		FIPv4Endpoint Endpoint(IPAddress, TCP_PORT);

		TcpSocket = FTcpSocketBuilder(TEXT("TcpSocket")).AsReusable().Build();
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		if (TcpSocket -> Connect(*SocketSubsystem->CreateInternetAddr(Endpoint.Address.Value, Endpoint.Port)))
		{
			// Sends a message to gama to confirm it's connected
			FString connection_str = FString("connected\n");
			int32 BytesSent;
			if (TcpSocket -> Send((const uint8*)  TCHAR_TO_ANSI(*connection_str), connection_str.Len(), BytesSent))
			{
				tcp_connected = true;
			}
		}
	}

	

	if (tcp_connected)
	{
		uint32 BufferSize;
		if (TcpSocket -> HasPendingData(BufferSize))
		{
			uint8* DataChunk = new uint8[BufferSize+1];
			int32 BytesRead;
			if (TcpSocket -> Recv(DataChunk, BufferSize, BytesRead) && BytesRead > 0)
			{
				DataChunk[BufferSize] = '\0'; // Hack
				FString message = UTF8_TO_TCHAR(DataChunk);

				UE_LOG(LogTemp, Display, TEXT("%s"), *message);

				TSharedPtr<FJsonObject> MyJson;

				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*message);

				if (FJsonSerializer::Deserialize(Reader, MyJson))
				{
					// The deserialization failed, handle this case
					// UE_LOG(LogTemp, Display, TEXT("Unable to deserialize"))
					ObjHandler->HandleObject(MyJson, GetWorld());
				}

				
				// Cleaning the message from everything after the final \n
				//message.RemoveFromEnd("\n");
			}
			delete[] DataChunk;
		}
		else
		{
			// Unable to read for some reason
		}

		// Randomly picks a house and turns it into an office
		/*if (ObjHandler->Get_House_Ids().Num() > 0 && FMath::FRand() > 0.99)
		{
			auto idx = FMath::RandRange(0, ObjHandler->Get_House_Ids().Num()-1);
			int id = ObjHandler->Get_House_Ids()[idx];
			// Sends a message to gama to confirm it's connected
			FString change_msg = FString("{ \"type\": \"house\", \"id\": ") + FString::FromInt(id) + FString("}\n");
			int32 BytesSent;
			TcpSocket -> Send((const uint8*)  TCHAR_TO_ANSI(*change_msg), change_msg.Len(), BytesSent);
		}*/
		
	}
	// step/stepBack command
	// expression command
	// if (message_handler -> GetExpId() > 0)
	// {
	// 	client -> step(message_handler -> GetSocketId(), message_handler -> GetExpId(), 2, false);
	// 	client -> expression(message_handler -> GetSocketId(), message_handler -> GetExpId(), "length(prey)");
	// 	// client -> stepBack(message_handler -> GetSocketId(), message_handler -> GetExpId(), 1, false);
	// 	// client -> expression(message_handler -> GetSocketId(), message_handler -> GetExpId(), "length(predator)");
	// }
}

void AGamaActions::SendChange(FString type, int32 ID)
{
	FString change_msg = FString("{ \"type\": \"") + type + FString("\", \"id\": ") + FString::FromInt(ID) + FString("}\n");
	int32 BytesSent;
	TcpSocket -> Send((const uint8*)  TCHAR_TO_ANSI(*change_msg), change_msg.Len(), BytesSent);
}

