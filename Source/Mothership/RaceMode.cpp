// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "RaceMode.h"

#include "PlayerShipController.h"


ARaceMode::ARaceMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = APlayerShipController::StaticClass();

	// set default pawn class to the blueprinted ship
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Ships/DefaultShip/DefaultShip"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if(PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ARaceMode::RegisterCheckpointTime(int32 PlayerId, float Time, bool StartFinish)
{
	auto& Data = PlayerData.FindOrAdd(PlayerId);
	Data.CheckpointPasses.Add({ Time, StartFinish });

	// Compute last sector time
	if(Data.CheckpointPasses.Num() > 1)
	{
		Data.LastSectorTime = Time - Data.CheckpointPasses[Data.CheckpointPasses.Num() - 2].Time;
	}

	if(StartFinish)
	{
		// Compute last lap time
		for(int32 i = Data.CheckpointPasses.Num() - 2; i >= 0; --i)
		{
			if(Data.CheckpointPasses[i].StartFinish)
			{
				float LastLap = Data.CheckpointPasses[i].Time;
				Data.LastLapTime = Time - LastLap;
				break;
			}
		}

		++Data.Lap;
	}
}

int32 ARaceMode::GetCurrentLap(int32 PlayerId)
{
	return PlayerData.FindOrAdd(PlayerId).Lap;
}

float ARaceMode::GetLastLapTime(int32 PlayerId)
{
	return PlayerData.FindOrAdd(PlayerId).LastLapTime;
}

float ARaceMode::GetLastSectorTime(int32 PlayerId)
{
	return PlayerData.FindOrAdd(PlayerId).LastSectorTime;
}