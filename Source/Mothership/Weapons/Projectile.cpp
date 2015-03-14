// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "Projectile.h"
#include "../Helper/Utilities.h"
#include "../HealthComponent.h"

#include "Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UnrealNetwork.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->MaxSpeed = 1000000.f;
	
	InitialLifeSpan = 3.f;

	this->bReplicates = true;
	this->bReplicateMovement = true;

	NetCullDistanceSquared = 10000000000.f;		///< Visible up to 1000m away (seems to be sufficient)
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(PhysicalRepresentation)
	{
		ProjectileMovement->SetUpdatedComponent(PhysicalRepresentation);
	}
}

void AProjectile::ReceiveHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Okay, target reached (Hopefully...)!
	if(Other && Role == ROLE_Authority)
	{
		Other->TakeDamage(DamageAmount, FDamageEvent(DamageType), this->GetInstigatorController(), this);
	}

	this->Destroy();
}

UPrimitiveComponent* AProjectile::GetPhysicalRepresentation()
{
	return PhysicalRepresentation;
}