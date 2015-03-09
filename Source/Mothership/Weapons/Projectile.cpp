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
		PhysicalRepresentation->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnEndOverlap);
		//if(AActor* Owner = GetOwner()) 
		//{
		//	if(UWorld* World = GetWorld())
		//	{
		//		TArray<FOverlapResult> Overlaps;
		//		FComponentQueryParams QueryParams;
		//		QueryParams.AddIgnoredActor(this);
		//		QueryParams.bFindInitialOverlaps = true;
		//		FCollisionObjectQueryParams CollisionParams;
		//		
		//		World->ComponentOverlapMulti(Overlaps,
		//			PhysicalRepresentation,
		//			PhysicalRepresentation->GetComponentLocation(),
		//			PhysicalRepresentation->GetComponentRotation(),
		//			QueryParams);

		//		bool OverlapsOwner = false;
		//		for(auto& Overlap : Overlaps)
		//		{
		//			if(Overlap.Actor.Get() == Owner)
		//			{
		//				// Overlaps with owner -> Enable Collision as soon as this is no longer true
		//				OverlapsOwner = true;
		//				break;
		//			}
		//		}

		//		if(OverlapsOwner)
		//		{
		//			PhysicalRepresentation->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnEndOverlap);
		//		}
		//		else
		//		{
		//			EnableCollision();
		//		}
		//	}
		//}
	}

}

void AProjectile::OnEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(GetOwner() == OtherActor)
	{
		EnableCollision();
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