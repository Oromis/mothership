// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerShipController.generated.h"

class AShipPawn;

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
	// End PlayerController interface
	
	void OnThrottleInput(float Value);
	void OnDirectionInput(float Value);

	// ------------------------------------------------------------------------------------
	// Client only
	// ------------------------------------------------------------------------------------

	UFUNCTION(Reliable, Client)
	void ClientSetSpectatorCamera(FVector CameraLocation, FRotator CameraRotation);

	UPROPERTY()
	AShipPawn* Ship = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mechanics)
	float RespawnTime = 5.f;

private:
	FTimerHandle RespawnTimerHandle;
};
