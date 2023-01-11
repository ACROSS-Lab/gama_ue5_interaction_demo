// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class GAMAUE5INTERACTION_API ExpParameter
{
private:
	FString type;
	FString value;
	FString name;
public:
	ExpParameter();
	ExpParameter(FString Type, FString Value, FString Name);
	FString Convert();
	~ExpParameter();
};
