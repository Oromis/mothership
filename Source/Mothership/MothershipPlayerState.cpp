// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "MothershipPlayerState.h"

#include "Engine.h"
#include "UnrealNetwork.h"


FORCEINLINE float AMothershipPlayerState::GetRespawnTime()
{
	return RespawnTime;
}

void AMothershipPlayerState::SetRespawnTime(float Time)
{
	this->RespawnTime = Time;
}

void AMothershipPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMothershipPlayerState, SelectedPawn);
	DOREPLIFETIME(AMothershipPlayerState, MayRespawn);
	DOREPLIFETIME(AMothershipPlayerState, PlayerStatus);
	DOREPLIFETIME(AMothershipPlayerState, Kills);
	DOREPLIFETIME(AMothershipPlayerState, Deaths);
	DOREPLIFETIME(AMothershipPlayerState, RespawnTime);
}