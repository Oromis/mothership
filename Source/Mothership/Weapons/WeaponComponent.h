// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Weapons), BlueprintType, meta=(BlueprintSpawnableComponent) )
class MOTHERSHIP_API UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

	/// Fires the weapon. Returns true if successful and false otherwise.
	virtual bool Fire();

	/// Actually fires the weapon on the server
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerFire();
	virtual void ServerFire_Implementation();
	virtual bool ServerFire_Validate();

	/// returns true if the weapon could fire right now, false otherwise (due to cooldown, no ammo, ...)
	virtual bool CanFire();

	/// Returns the time in seconds until the weapon can fire its next shot. A value <= 0 means it can fire right now.
	virtual float GetTimeUntilNextShot();

protected:

	/// Number of shots per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons)
	float RateOfFire = 1.f;

	/// Gametime when the last shot was fired
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Weapons)
	float LastShotTime = 0.f;
};
