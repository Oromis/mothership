// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponComponent.h"
#include "ProjectileWeaponComponent.generated.h"

class AProjectile;

UCLASS(ClassGroup = (Weapons), BlueprintType, meta = (BlueprintSpawnableComponent))
class MOTHERSHIP_API UProjectileWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	UProjectileWeaponComponent();

	virtual void InitializeComponent() override;

	virtual void ServerFire_Implementation() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	TSubclassOf<AActor> ProjectileClass;
};
