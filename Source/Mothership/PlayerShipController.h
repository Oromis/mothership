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

	UFUNCTION(Reliable, Server, WithValidation)
	virtual void ServerRespawnPlayer();

	// ------------------------------------------------------------------------------------
	// Client only
	// ------------------------------------------------------------------------------------

	UFUNCTION(Reliable, Client)
	virtual void ClientSetSpectatorCamera(FVector CameraLocation, FRotator CameraRotation);

	// ------------------------------------------------------------------------------------
	// Properties
	// ------------------------------------------------------------------------------------

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Owner)
	AShipPawn* Ship = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Mechanics)
	AMothershipPlayerState* MSPlayerState = nullptr;

private:

};
