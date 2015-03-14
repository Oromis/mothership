// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitDelegate, FVector, HitLocation);

class UProjectileMovementComponent;

UCLASS()
class MOTHERSHIP_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void BeginPlay() override;

	virtual void ReceiveHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit) override;

	UPrimitiveComponent* GetPhysicalRepresentation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	TSubclassOf<UDamageType> DamageType;

protected:
	/// The component with the collision shape. Could be a mesh or a sphere or a capsule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physical)
	UPrimitiveComponent* PhysicalRepresentation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physical)
	UProjectileMovementComponent* ProjectileMovement;
};
