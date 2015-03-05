// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "PlayerShipController.h"
#include "ShipPawn.h"
#include "MothershipPlayerState.h"
#include "MothershipGameMode.h"

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

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerShipController::OnFirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerShipController::OnFireReleased);

	InputComponent->BindAction("Respawn", IE_Pressed, this, &APlayerShipController::OnRespawnPressed);
}

void APlayerShipController::SetPawn(APawn* NewPawn)
{
	Super::SetPawn(NewPawn);

	Ship = NewPawn ? Cast<AShipPawn>(NewPawn) : nullptr;
}

void APlayerShipController::InitPlayerState()
{
	Super::InitPlayerState();
}

void APlayerShipController::OnThrottleInput(float Value)
{
	if(Ship)
	{
		if(Ship->GetThrottleControl() != Value)
		{
			Ship->SetThrottleControl(Value);
			ServerSetThrottle(Value);
		}
	}
}

void APlayerShipController::OnDirectionInput(float Value)
{
	if(Ship)
	{
		if(Ship->GetDirectionControl() != Value)
		{
			Ship->SetDirectionControl(Value);
			ServerSetDirection(Value);
		}
	}
}

void APlayerShipController::OnFirePressed()
{
	if(Ship)
	{
		Ship->SetFiring(true);
	}
}

void APlayerShipController::OnFireReleased()
{
	if(Ship)
	{
		Ship->SetFiring(false);
	}
}

void APlayerShipController::OnRespawnPressed()
{
	if(AMothershipPlayerState* State = Cast<AMothershipPlayerState>(PlayerState))
	{
		if(State->PlayerStatus == FPlayerStatus::WATCHING && State->MayRespawn)
		{
			ServerRespawnPlayer();
		}
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

void APlayerShipController::ServerRespawnPlayer_Implementation()
{
	GetWorld()->GetAuthGameMode<AMothershipGameMode>()->RestartPlayer(this);
}

bool APlayerShipController::ServerRespawnPlayer_Validate()
{
	return true;
}

void APlayerShipController::ServerSetThrottle_Implementation(float Value)
{
	if(Ship)
	{
		Ship->SetThrottleControl(Value);
	}
}

bool APlayerShipController::ServerSetThrottle_Validate(float Value)
{
	return -1.f <= Value && Value <= 1.f;
}

void APlayerShipController::ServerSetDirection_Implementation(float Value)
{
	if(Ship)
	{
		Ship->SetDirectionControl(Value);
	}
}

bool APlayerShipController::ServerSetDirection_Validate(float Value)
{
	return -1.f <= Value && Value <= 1.f;
}

void APlayerShipController::ClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation)
{
	this->SetInitialLocationAndRotation(CameraLocation, CameraRotation);
	this->SetViewTarget(this);
}
