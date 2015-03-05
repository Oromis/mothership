// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class MOTHERSHIP_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void BeginPlay() override;

	void SetInitialVelocity(float InitialVelocity);

protected:
	/// The component with the collision shape. Could be a mesh or a sphere or a capsule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physical)
	USceneComponent* PhysicalRepresentation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physical)
	UProjectileMovementComponent* ProjectileMovement;
};
