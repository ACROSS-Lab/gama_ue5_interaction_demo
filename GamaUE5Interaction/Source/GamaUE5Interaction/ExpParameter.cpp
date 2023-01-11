// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpParameter.h"


ExpParameter::ExpParameter()
{
}

ExpParameter::ExpParameter(FString Type, FString Value, FString Name)
{
    type = Type;
    value = Value;
    name = Name;
}

FString ExpParameter::Convert()
{
    FString parameter = ("\
    {\
        \"type\": \"" + type + "\",\
        \"value\": \"" + value + "\",\
        \"name\": \"" + name + "\"\
    }\n");
    return parameter;
}

ExpParameter::~ExpParameter()
{
}
