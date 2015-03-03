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
	// End PlayerController interface
	
	void OnThrottleInput(float Value);
	void OnDirectionInput(float Value);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Owner)
	AShipPawn* Ship = nullptr;
};
