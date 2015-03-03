// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "MothershipGameMode.generated.h"

UCLASS(minimalapi)
class AMothershipGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMothershipGameMode(const FObjectInitializer& ObjectInitializer);

	virtual UClass* GetDefaultPawnClassForController(AController* InController) override;
};



