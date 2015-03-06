// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "ProjectileWeaponComponent.h"
#include "Projectile.h"

#include "Engine.h"
#include "UnrealNetwork.h"

UProjectileWeaponComponent::UProjectileWeaponComponent()
{
	
}

void UProjectileWeaponComponent::ServerFire_Implementation()
{
	if(CanFire())
	{
		if(UWorld* World = GetWorld())
		{
			if(*ProjectileClass)
			{
				FActorSpawnParameters SpawnParams;
				if(APawn* OwnerPawn = Cast<APawn>(GetOwner()))
				{
					SpawnParams.Instigator = OwnerPawn;
				}
				SpawnParams.Owner = GetOwner();

				AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, 
					this->GetComponentLocation(), 
					this->GetComponentRotation(), 
					SpawnParams);
				if(Projectile)
				{
					Projectile->SetInitialVelocity(InitialProjectileSpeed);
				}
			}
		}
		Super::ServerFire_Implementation();
	}
}