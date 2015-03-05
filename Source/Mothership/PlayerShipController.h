// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerShipController.generated.h"

class AShipPawn;
class AMothershipPlayerState;

/**
 * Controller for a one-man ship
 */
UCLASS()
class MOTHERSHIP_API APlayerShipController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* NewPawn) override;
	
	virtual void PawnPendingDestroy(APawn* Pawn) override;
	virtual void InitPlayerState() override;
	// End PlayerController interface
	
	void OnThrottleInput(float Value);
	void OnDirectionInput(float Value);

	void OnRespawnPressed();

	// ------------------------------------------------------------------------------------
	// Server only
	// ------------------------------------------------------------------------------------

	//virtual void ServerRespawnPlayer_Implementation();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRespawnPlayer();

	//virtual void ServerSetThrottle_Implementation(float Value);
	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetThrottle(float Value);

	//virtual void ServerSetDirection_Implementation(float Value);
	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetDirection(float Value);

	// ------------------------------------------------------------------------------------
	// Client only
	// ------------------------------------------------------------------------------------

	UFUNCTION(Reliable, Client)
	void ClientSetSpectatorCamera(FVector CameraLocation, FRotator CameraRotation);
	void ClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation);

	// ------------------------------------------------------------------------------------
	// Properties
	// ------------------------------------------------------------------------------------

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Owner)
	AShipPawn* Ship = nullptr;

private:

};
