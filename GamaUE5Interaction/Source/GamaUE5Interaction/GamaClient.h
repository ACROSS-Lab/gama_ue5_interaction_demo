// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "CoreMinimal.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "Containers/Array.h"
#include "MessageHandler.h"
#include "GamaActionsMessageHandler.h"
#include "ExpParameter.h"


class GAMAUE5INTERACTION_API GamaClient
{
private: 
	TSharedPtr<IWebSocket> Socket;
	// int64 socket_id;
	MessageHandler* message_handler;
public:
	GamaClient();
	GamaClient(FString url, int32 port, MessageHandler* message_handler);
	void connect();
	bool IsConnected();
	void exit();
	void load(int64 socket_id, FString file_path, FString experiment_name, bool console = true, bool status = false, bool dialog = false, TArray<ExpParameter*> parameters = TArray<ExpParameter*>(), FString end_condition = "");
	void play(int64 socket_id, int32 exp_id, bool sync = false);
	void pause(int64 socket_id, int32 exp_id);
	void step(int64 socket_id, int32 exp_id, int32 steps = 1, bool sync = false);
	void stepBack(int64 socket_id, int32 exp_id, int32 steps = 1, bool sync = false);
	void stop(int64 socket_id, int32 exp_id);
	void reload(int64 socket_id, int32 exp_id, TArray<ExpParameter*> parameters = TArray<ExpParameter*>(), FString end_condition = "");
	void expression(int64 socket_id, int32 exp_id, FString expr);
	virtual ~GamaClient();
};
