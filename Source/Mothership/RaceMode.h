// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "RaceMode.generated.h"

/**
 * A fun, arcady game mode where the spaceships race on a space track
 */
UCLASS()
class MOTHERSHIP_API ARaceMode : public AGameMode
{
	GENERATED_BODY()

	struct CheckpointPass
	{
		float Time;
		bool StartFinish;
	};

	struct PlayerData
	{
		int32 Lap = 0;
		float LastLapTime = 0.f;
		float LastSectorTime = 0.f;
		TArray<CheckpointPass> CheckpointPasses;
	};
	
public:
	ARaceMode(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = Race)
	void RegisterCheckpointTime(int32 PlayerId, float Time, bool StartFinish);

	UFUNCTION(BlueprintCallable, Category = Race)
	int32 GetCurrentLap(int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category = Race)
	float GetLastLapTime(int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category = Race)
	float GetLastSectorTime(int32 PlayerId);

private:

	TMap<int32, PlayerData> PlayerData;
};
