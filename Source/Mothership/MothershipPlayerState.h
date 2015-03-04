// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "MothershipPlayerState.generated.h"

UENUM(BlueprintType)
enum class FPlayerStatus : uint8
{
	PLAYING,
	WATCHING,
};

/**
 * Contains player-specific data that needs to be replicated to every client
 */
UCLASS()
class MOTHERSHIP_API AMothershipPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	float GetRespawnTime();
	void SetRespawnTime(float Time);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mechanics)
	TSubclassOf<APawn> SelectedPawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Mechanics)
	bool MayRespawn = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mechanics)
	FPlayerStatus PlayerStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Score)
	int32 Kills;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Score)
	int32 Deaths;

protected:
	/// Minimum Time until respawn in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mechanics)
	float RespawnTime = 5.f;
};
