// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "MothershipGameMode.generated.h"

struct FPlayerData
{
	FTimerHandle RespawnTimer;
	AController* Controller;
};

UCLASS(minimalapi)
class AMothershipGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMothershipGameMode(const FObjectInitializer& ObjectInitializer);

	virtual UClass* GetDefaultPawnClassForController(AController* InController) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, const FString& Portal, const FString& Options, const TSharedPtr<class FUniqueNetId>& UniqueId, FString& ErrorMessage) override;

	void OnKill(AController* Killer, AController* Victim, APawn* KilledPawn);

	void OnRespawnTimerExpired();

	/// Respawns a player
	virtual void RestartPlayer(AController* NewPlayer);

protected:
	TMap<int32, FPlayerData> PlayerData;
};



