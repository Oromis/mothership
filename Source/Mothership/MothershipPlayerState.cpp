// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "MothershipPlayerState.h"


FORCEINLINE float AMothershipPlayerState::GetRespawnTime()
{
	return RespawnTime;
}

void AMothershipPlayerState::SetRespawnTime(float Time)
{
	this->RespawnTime = Time;
}