// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mothership.h"
#include "PlayerShipController.h"
#include "MothershipGameMode.h"
#include "MothershipPlayerState.h"
#include "ShipPawn.h"

#include "GameFramework/PlayerState.h"
#include "GameFramework/HUD.h"

AMothershipGameMode::AMothershipGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = APlayerShipController::StaticClass();
	PlayerStateClass = AMothershipPlayerState::StaticClass();

	// set default pawn class to the blueprinted ship
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Ships/DefaultShip/DefaultShip"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	this->bStartPlayersAsSpectators = true;

	//set the default HUD class
	static ConstructorHelpers::FClassFinder<AHUD> DefaultHUDClass(TEXT("/Game/Blueprints/HUD/DefaultHUD"));
	HUDClass = DefaultHUDClass.Class;
}

UClass* AMothershipGameMode::GetDefaultPawnClassForController(AController* InController)
{
	if(AMothershipPlayerState* State = Cast<AMothershipPlayerState>(InController->PlayerState))
	{
		if(State->SelectedPawn)
		{
			return State->SelectedPawn;
		}
	}
	return Super::GetDefaultPawnClassForController(InController);
}

APlayerController* AMothershipGameMode::Login(class UPlayer* NewPlayer, const FString& Portal, const FString& Options, 
	const TSharedPtr<class FUniqueNetId>& UniqueId, FString& ErrorMessage)
{
	APlayerController* NewController = Super::Login(NewPlayer, Portal, Options, UniqueId, ErrorMessage);
	FPlayerData Data;
	Data.Controller = NewController;
	PlayerData.Add(NewController->PlayerState->PlayerId, Data);
	return NewController;
}

void AMothershipGameMode::OnKill(AController* Killer, AController* Victim, APawn* KilledPawn)
{
	if(Victim)
	{
		// Set a timer that allows the player to respawn
		if(AMothershipPlayerState* VictimState = Cast<AMothershipPlayerState>(Victim->PlayerState))
		{
			FTimerManager& TimerManager = GetWorldTimerManager();
			FPlayerData& Data = PlayerData.FindOrAdd(Victim->PlayerState->PlayerId);
			TimerManager.SetTimer(Data.RespawnTimer, this, &AMothershipGameMode::OnRespawnTimerExpired, VictimState->GetRespawnTime());

			VictimState->PlayerStatus = FPlayerStatus::WATCHING;
			++VictimState->Deaths;
		}
	}
	if(Killer)
	{
		if(AMothershipPlayerState* KillerState = Cast<AMothershipPlayerState>(Killer->PlayerState))
		{
			++KillerState->Kills;
		}
	}
}

void AMothershipGameMode::OnRespawnTimerExpired()
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	for(auto& Entry : PlayerData)
	{
		if(AMothershipPlayerState* PlayerState = Cast<AMothershipPlayerState>(Entry.Value.Controller->PlayerState))
		{
			if(PlayerState->PlayerStatus == FPlayerStatus::WATCHING && !TimerManager.IsTimerActive(Entry.Value.RespawnTimer))
			{
				PlayerState->MayRespawn = true;
			}
		}
	}
}