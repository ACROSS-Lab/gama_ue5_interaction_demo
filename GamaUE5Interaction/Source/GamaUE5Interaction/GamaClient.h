// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IWebSocket.h"
#include "Containers/Array.h"
#include "MessageHandler.h"
#include "ExpParameter.h"


class GAMAUE5INTERACTION_API GamaClient
{
private: 
	TSharedPtr<IWebSocket> Socket;
	MessageHandler* message_handler;
public:
	GamaClient();
	GamaClient(FString url, int32 port, MessageHandler* message_handler);
	void connect() const;
	bool IsConnected() const;

	// basic Gama server commands
	void exit() const;
	void load(int64 socket_id, FString file_path, FString experiment_name, bool console = true, bool status = false, bool dialog = false, TArray<ExpParameter*> parameters = TArray<ExpParameter*>(), FString end_condition = "") const;
	void play(int64 socket_id, int32 exp_id, bool sync = false) const;
	void pause(int64 socket_id, int32 exp_id) const;
	void step(int64 socket_id, int32 exp_id, int32 steps = 1, bool sync = false) const;
	void stepBack(int64 socket_id, int32 exp_id, int32 steps = 1, bool sync = false) const;
	void stop(int64 socket_id, int32 exp_id) const;
	void reload(int64 socket_id, int32 exp_id, TArray<ExpParameter*> parameters = TArray<ExpParameter*>(), FString end_condition = "") const;
	void expression(int64 socket_id, int32 exp_id, FString expr) const;

	virtual ~GamaClient();
};
