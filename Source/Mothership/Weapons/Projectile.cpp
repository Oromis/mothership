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

	//PhysicalRepresentation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	//PhysicalRepresentation->SetCollisionProfileName(TEXT("BlockAll"));
	//RootComponent = PhysicalRepresentation;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->MaxSpeed = 1000000.f;
	//ProjectileMovement->SetUpdatedComponent(PhysicalRepresentation);
	
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

	if(AActor* Owner = GetOwner())
	{
		if(PhysicalRepresentation)
		{
			// Ignore my owner
			PhysicalRepresentation->IgnoreActorWhenMoving(Owner, true);
		}

		if(UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
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

	this->Destroy();
}

UPrimitiveComponent* AProjectile::GetPhysicalRepresentation()
{
	return PhysicalRepresentation;
}