// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <string>
#include "CoreMinimal.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "MessageHandler.h"
#include "GamaActionsMessageHandler.h"


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
	void load(int64 socket_id, FString file_path, FString experiment_name, bool console, bool status, bool dialog);
	void play(int64 socket_id, int32 exp_id, bool sync);
	void pause(int64 socket_id, int32 exp_id);
	void step(int64 socket_id, int32 exp_id, int32 steps, bool sync);
	void stepBack(int64 socket_id, int32 exp_id, int32 steps, bool sync);
	void stop(int64 socket_id, int32 exp_id);
	void reload(int64 socket_id, int32 exp_id);
	void expression(int64 socket_id, int32 exp_id, FString expr);
	~GamaClient();
};
