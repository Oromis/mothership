// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mothership.h"
#include "MothershipGameMode.h"
#include "ShipPawn.h"
#include "PlayerShipController.h"

AMothershipGameMode::AMothershipGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = APlayerShipController::StaticClass();

	// set default pawn class to the blueprinted ship
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Ships/DefaultShip/DefaultShip"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}