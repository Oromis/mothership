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

void APlayerShipController::PawnPendingDestroy(APawn* Pawn)
{
	if(Ship == Pawn)
	{
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();

		Super::PawnPendingDestroy(Pawn);

		ClientSetSpectatorCamera(CameraLocation, CameraRotation);
	}
}

void APlayerShipController::ClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation)
{
	this->SetInitialLocationAndRotation(CameraLocation, CameraRotation);
	this->SetViewTarget(this);
}
