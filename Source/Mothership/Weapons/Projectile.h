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

	UFUNCTION(NetMulticast, Unreliable)
	void SetInitialVelocity(float InitialVelocity);
	void SetInitialVelocity_Implementation(float InitialVelocity);

	virtual void ReceiveHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit) override;

	UPrimitiveComponent* GetPhysicalRepresentation();

	UFUNCTION(BlueprintImplementableEvent)
	void EnableCollision();

	UFUNCTION()
	void OnEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	/// The component with the collision shape. Could be a mesh or a sphere or a capsule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physical)
	UPrimitiveComponent* PhysicalRepresentation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Physical)
	UProjectileMovementComponent* ProjectileMovement;
};
