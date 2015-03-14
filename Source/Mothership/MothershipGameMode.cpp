// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Mothership.h"
#include "PlayerShipController.h"
#include "MothershipGameMode.h"
#include "MothershipPlayerState.h"
#include "ShipPawn.h"

#include "GameFramework/PlayerState.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerStart.h"

#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogMotherShipGameMode, Log, All);

AMothershipGameMode::AMothershipGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = APlayerShipController::StaticClass();
	PlayerStateClass = AMothershipPlayerState::StaticClass();

	// set default pawn class to the blueprinted ship
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Ships/DefaultShip/DefaultShip"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Ships/Scorpion/Scorpion"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	this->bStartPlayersAsSpectators = false;
	
	//We won't be ticked by default  
	PrimaryActorTick.bCanEverTick = true; 

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

void AMothershipGameMode::RestartPlayer(AController* Player)
{
	if(AMothershipPlayerState* PlayerState = Cast<AMothershipPlayerState>(Player->PlayerState))
	{
		if(PlayerState->MayRespawn)
		{
			PlayerState->MayRespawn = false;
			Super::RestartPlayer(Player);
		}
	}
}

AActor* AMothershipGameMode::ChoosePlayerStart(AController* Player)
{
	// Choose a player start
	APlayerStart* FoundPlayerStart = NULL;
	APawn* PawnToFit = DefaultPawnClass ? DefaultPawnClass->GetDefaultObject<APawn>() : NULL;
	TArray<APlayerStart*> UnOccupiedStartPoints;
	TArray<APlayerStart*> OccupiedStartPoints;
	for (int32 PlayerStartIndex = 0; PlayerStartIndex < PlayerStarts.Num(); ++PlayerStartIndex)
	{
		APlayerStart* PlayerStart = PlayerStarts[PlayerStartIndex];

		if (Cast<APlayerStartPIE>(PlayerStart) != NULL)
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			FoundPlayerStart = PlayerStart;
			break;
		}
		else if (PlayerStart != NULL)
		{
			FVector ActorLocation = PlayerStart->GetActorLocation();
			const FRotator ActorRotation = PlayerStart->GetActorRotation();
			if (!GetWorld()->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
			{
				if (!UsedPlayerStarts.Contains(PlayerStart))
				{
					UnOccupiedStartPoints.Add(PlayerStart);
				}
			}
			else if (GetWorld()->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				OccupiedStartPoints.Add(PlayerStart);
			}
		}
	}
	if (FoundPlayerStart == NULL)
	{
		if (UnOccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}

	// remember the player starts we already used in this tick 
	// so no two ships spawn at the same location (in one tick)
	this->UsedPlayerStarts.Add(FoundPlayerStart);
	UE_LOG(LogMotherShipGameMode, Warning, TEXT("Found a spawnpoint"));
	SpawnedPlayers = true;

	return FoundPlayerStart;
}

void AMothershipGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// we can clear the spawns we used in the last tick if we used some
	if (this->SpawnedPlayers) 
	{
		this->UsedPlayerStarts.Empty();
		SpawnedPlayers = false;
	}
}