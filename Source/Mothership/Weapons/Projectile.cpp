// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "Projectile.h"
#include "../Helper/Utilities.h"

#include "Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	
	InitialLifeSpan = 3.f;

	this->bReplicates = true;
	this->bReplicateMovement = true;

	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);

	NetCullDistanceSquared = 10000000000.f;		///< Visible up to 1000m away (seems to be sufficient)
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(PhysicalRepresentation)
	{
		ProjectileMovement->SetUpdatedComponent(PhysicalRepresentation);
	}

	if(AActor* Owner = GetOwner())
	{
		if(PhysicalRepresentation)
		{
			// Ignore my owner
			PhysicalRepresentation->IgnoreActorWhenMoving(Owner, true);
		}

		if(UPrimitiveComponent* Root = Owner->GetRootPrimitiveComponent())
		{
			// Owner should ignore me
			Root->IgnoreActorWhenMoving(this, true);
		}
	}
}

void AProjectile::SetInitialVelocity_Implementation(float InitialVelocity)
{
	ProjectileMovement->InitialSpeed = InitialVelocity;
	ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetUnsafeNormal() * InitialVelocity;
}

void AProjectile::ReceiveHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}