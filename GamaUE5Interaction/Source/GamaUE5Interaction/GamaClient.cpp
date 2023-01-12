// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaClient.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "Json.h"
#include "jsoncpp/json/value.h"
#include "jsoncpp/json/json.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Serialization/JsonSerializer.h"
#include "Containers/Array.h"
#include "MessageHandler.h"
#include "ExpParameter.h"


GamaClient::GamaClient()
{
}

GamaClient::GamaClient(FString url, int32 port, MessageHandler* message_handler) 
{
    this -> message_handler = message_handler;
    const FString ServerURL = FString("ws://") + url + FString(":") + FString(std::to_string(port).c_str()); // Your server URL. You can use ws, wss or wss+insecure.
    const FString ServerProtocol = FString("ws");              // The WebServer protocol you want to use.
        
    Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
}

bool GamaClient::IsConnected()
{
    return Socket -> IsConnected();
}

void GamaClient::connect()
{
    // We bind all available events
    Socket->OnConnected().AddLambda([]() -> void {
        // This code will run once connected.
        UE_LOG(LogTemp, Display, TEXT("connected"));
    });
        
    Socket->OnConnectionError().AddLambda([](const FString & Error) -> void {
        // This code will run if the connection failed. Check Error to see what happened.
        UE_LOG(LogTemp, Display, TEXT("Something bad happened while trying to connect"));
       // UE_LOG(LogTemp, Display, Error);
    });
        
    Socket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean) -> void {
        // This code will run when the connection to the server has been terminated.
        // Because of an error or a call to Socket->Close().
        UE_LOG(LogTemp, Display, TEXT("Connection closed"));
       // UE_LOG(LogTemp, Display, Reason);
    });
        
    Socket->OnMessage().AddLambda([&](const FString & Message) -> void {

        // This code will run when we receive a string message from the server.
        UE_LOG(LogTemp, Display, TEXT("Message received: %s"), *FString(Message));

        TSharedPtr<FJsonObject> MyJson;

        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*Message);

        if (FJsonSerializer::Deserialize(Reader, MyJson))
        {
            // The deserialization failed, handle this case
            // UE_LOG(LogTemp, Display, TEXT("Unable to deserialize"))
            if (MyJson->GetField<EJson::String>(FString("type")) -> Type != EJson::Null) {
                message_handler -> HandleCommand(MyJson);
            }
        }
    });
        
    Socket->OnRawMessage().AddLambda([](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) -> void {
        // This code will run when we receive a raw (binary) message from the server.
    });
        
    Socket->OnMessageSent().AddLambda([](const FString& MessageString) -> void {
        // This code is called after we sent a message to the server.
        UE_LOG(LogTemp, Display, TEXT("sent: %s"), *FString(MessageString));
    });
        
    // And we finally connect to the server. 
    Socket->Connect();
}

void GamaClient::exit()
{
    if(!Socket -> IsConnected())
    {
        return;
    }

    FString exit_command = FString("\
    {\
        \"type\": \"exit\"\
    }\n");

    Socket -> Send(exit_command);
}

void GamaClient::load(int64 socket_id, FString file_path, FString experiment_name, bool console = true, bool status = false, bool dialog = false, TArray<ExpParameter*> parameters = [], FString end_condition = "")
{
    if (!Socket->IsConnected())
    {
        // Don't send if we're not connected.
        return;
    }

    FString params = FString("[");
    
    for(int i = 0; i < parameters.Num(); i++)
    {
        params += parameters[i] -> Convert();
        if(i != parameters.Num() - 1) params += (FString) ", ";
    }

    params += FString("]");

    FString load_command = FString("    \
    {\
        \"type\": \"load\",\
        \"socket_id\": \"") + FString(std::to_string(socket_id).c_str()) + FString("\",\
        \"model\": \"") + file_path + FString("\",\
        \"experiment\": \"") + experiment_name + FString("\",\
        \"console\": ") + (console ? "true" : "false") + FString(",\
        \"status\": ") + (status ? "true" : "false") + FString(",\
        \"dialog\": ") + (dialog ? "true" : "false") + FString(",\
        \"parameters\": ") + params + FString(",\
        \"until\": \"") + end_condition + FString("\"\
    }\n") ;

    Socket -> Send(load_command);
}

void GamaClient::play(int64 socket_id, int32 exp_id, bool sync = false)
{
    if(!Socket -> IsConnected())
    {
        return;
    }

    FString play_command = FString("\
    {\
        \"type\": \"play\",\
        \"socket_id\": \"") + FString(std::to_string(socket_id).c_str()) + FString("\",\
        \"exp_id\": \"") + FString(std::to_string(exp_id).c_str()) +FString("\",\
        \"sync\": ") + (sync ? "true" : "false") + FString("\
    }");

    Socket -> Send(play_command);
}


void GamaClient::pause(int64 socket_id, int32 exp_id)
{
    if(!Socket -> IsConnected())
    {
        return;
    }

    FString pause_command = FString("\
    {\
        \"type\": \"pause\",\
        \"socket_id\": \"") + FString(std::to_string(socket_id).c_str()) + FString("\",\
        \"exp_id\": \"") + FString(std::to_string(exp_id).c_str()) + FString("\"\
    }");

    Socket -> Send(pause_command);
}

void GamaClient::step(int64 socket_id, int32 exp_id, int32 steps = 1, bool sync = false)
{
    if(!Socket -> IsConnected())
    {
        return;
    }

    FString step_command = FString("\
    {\
        \"type\": \"step\",\
        \"socket_id\": \"") + FString(std::to_string(socket_id).c_str()) + FString("\",\
        \"exp_id\": \"") + FString(std::to_string(exp_id).c_str()) + FString("\",\
        \"nb_step\": ") + FString(std::to_string(steps).c_str()) + FString(",\
        \"sync\": ") + (sync ? "true" : "false") + FString("\
    }");

    Socket -> Send(step_command);
}

void GamaClient::stepBack(int64 socket_id, int32 exp_id, int32 steps = 1, bool sync = false)
{
    if(!Socket -> IsConnected())
    {
        return;
    }

    FString step_command = FString("\
    {\
        \"type\": \"stepBack\",\
        \"socket_id\": \"") + FString(std::to_string(socket_id).c_str()) + FString("\",\
        \"exp_id\": \"") + FString(std::to_string(exp_id).c_str()) + FString("\",\
        \"nb_step\": ") + FString(std::to_string(steps).c_str()) + FString(",\
        \"sync\": ") + (sync ? "true" : "false") + FString("\
    }");

    Socket -> Send(step_command);
}

void GamaClient::stop(int64 socket_id, int32 exp_id)
{
    if(!Socket -> IsConnected())
    {
        return;
    }

    FString stop_command = FString("\
    {\
        \"type\": \"stop\",\
        \"socket_id\": \"") + FString(std::to_string(socket_id).c_str()) + FString("\",\
        \"exp_id\": \"") + FString(std::to_string(exp_id).c_str()) + FString("\"\
    }");

    Socket -> Send(stop_command);
}

void GamaClient::reload(int64 socket_id, int32 exp_id, TArray<ExpParameter*> parameters = [], FString end_condition = "")
{
    if(!Socket -> IsConnected())
    {
        return;
    }

    FString params = FString("[");
    
    for(int i = 0; i < parameters.Num(); i++)
    {
        params += parameters[i] -> Convert();
    }

    params += FString("]");

    FString reload_command = FString("\
    {\
        \"type\": \"reload\",\
        \"socket_id\": \"") + FString(std::to_string(socket_id).c_str()) + FString("\",\
        \"exp_id\": \"") + FString(std::to_string(exp_id).c_str()) + FString("\",\
        \"params\": ") + params + FString(",\
        \"until\": \"") + end_condition + FString("\"\
    }");

    Socket -> Send(reload_command);
}

void GamaClient::expression(int64 socket_id, int32 exp_id, FString expr)
{
    if(!Socket -> IsConnected())
    {
        return;
    }

    FString expression_command = FString("\
    {\
        \"type\": \"expression\",\
        \"socket_id\": \"") + FString(std::to_string(socket_id).c_str()) + FString("\",\
        \"exp_id\": \"") + FString(std::to_string(exp_id).c_str()) + FString("\",\
        \"expr\": \"") + expr + FString("\",\
    }");

    Socket -> Send(expression_command);
}

GamaClient::~GamaClient()
{
}
