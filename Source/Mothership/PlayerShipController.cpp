// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "PlayerShipController.h"
#include "ShipPawn.h"

void APlayerShipController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void APlayerShipController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	InputComponent->BindAxis("Throttle", this, &APlayerShipController::OnThrottleInput);
	InputComponent->BindAxis("Direction", this, &APlayerShipController::OnDirectionInput);
}

void APlayerShipController::SetPawn(APawn* NewPawn)
{
	Super::SetPawn(NewPawn);

	Ship = NewPawn ? Cast<AShipPawn>(NewPawn) : nullptr;
}

void APlayerShipController::OnThrottleInput(float Value)
{
	if(Ship)
	{
		Ship->SetThrottleControl(Value);
	}
}

void APlayerShipController::OnDirectionInput(float Value)
{
	if(Ship)
	{
		Ship->SetDirectionControl(Value);
	}
}