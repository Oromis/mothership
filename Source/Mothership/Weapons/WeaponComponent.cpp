	// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "WeaponComponent.h"

#include "Engine.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	bWantsInitializeComponent = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponComponent, LastShotTime);
}

bool UWeaponComponent::Fire()
{
	if(CanFire())
	{
		ServerFire();
		return true;
	}
	return false;
}

void UWeaponComponent::ServerFire_Implementation()
{
	// Reset cooldown. Actual firing implementation in subclasses
	if(CanFire())
	{
		if(UWorld* World = GetWorld())
		{
			LastShotTime = World->TimeSeconds;
		}
	}
}

bool UWeaponComponent::ServerFire_Validate()
{
	return true;
}

bool UWeaponComponent::CanFire()
{
	// Check rate of fire
	if(UWorld* World = GetWorld())
	{
		return LastShotTime + (1.f / RateOfFire) <= World->TimeSeconds;
	}

	// No world -> no fire
	return false;
}

float UWeaponComponent::GetTimeUntilNextShot()
{
	if(UWorld* World = GetWorld())
	{
		return World->TimeSeconds - (LastShotTime + (1.f / RateOfFire));
	}

	// No world -> Pls try again next second ;)
	return 1.f;
}