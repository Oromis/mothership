// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "Projectile.h"

#include "Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(PhysicalRepresentation)
	{
		ProjectileMovement->SetUpdatedComponent(PhysicalRepresentation);
	}
}

void AProjectile::SetInitialVelocity(float InitialVelocity)
{
	ProjectileMovement->InitialSpeed = InitialVelocity;
}