// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "ShipPawn.h"
#include "HealthComponent.h"
#include "DamageType/CrashDamage.h"
#include "Helper/Utilities.h"
#include "MothershipGameMode.h"

#include "Engine.h"
#include "UnrealNetwork.h"

namespace
{
	const float CRASH_DAMAGE_SCALE = 0.01f;
}

// Sets default values
AShipPawn::AShipPawn() :
	ForwardThrust(200.f),
	ReverseThrust(150.f),
	Friction(50.f),
	MaxSpeed(2000.f),
	TurnRate(900.f),
	CameraPitch(-40.f),
	MaxRollInCorners(90.f),
	RollRate(180.f),
	RotationStabilizerStrength(90.f),
	LinearStabilizerStrength(1600.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(true);

	bReplicates = true;
	bReplicateMovement = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->SetIsReplicated(true);

	bReplicates = true;
	bReplicateMovement = true; 
	NetCullDistanceSquared = 10000000000.f;		///< Visible up to 1000m away (seems to be sufficient)
}

// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
	Super::BeginPlay();

	if(CameraArm)
	{
		CameraArm->bAbsoluteRotation = true;
	}
}

// Called every frame
void AShipPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if(!IsDying)
	{
		MovementTick(DeltaTime);
	}
}

void AShipPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShipPawn, ThrottleControl);
	DOREPLIFETIME(AShipPawn, DirectionControl);
	DOREPLIFETIME(AShipPawn, Speed);
	DOREPLIFETIME(AShipPawn, DriftVelocity);
	DOREPLIFETIME(AShipPawn, HealthComponent);
}

void AShipPawn::ReceiveHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	//FVector DeltaVelocity = MyComp->GetComponentVelocity() - OtherComp->GetComponentVelocity();

	Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if(Mesh)
	{
		// Damage the ship
		if(Other && Role == ROLE_Authority)
		{
			float DamageScale = 1.f;
			float MeshMass = Mesh->GetMass();

			// More damage for the lighter body, less for the heavier one
			if(OtherComp && OtherComp->GetMass() != 0.f && MeshMass != 0.f)
			{
				DamageScale = OtherComp->GetMass() / MeshMass;
			}

			// Base Damage depends on the relative speed
			float BaseDamage;
			if(MeshMass)
			{
				// We have an Impulse. Impulse is Force over time [N*s] = [kg * m / s],
				// Therefore difference of speed is Imp / Mass [kg * m / s / kg] = [m / s].
				BaseDamage = NormalImpulse.Size() / MeshMass;
			}
			else
			{
				BaseDamage = FMath::Abs(Speed);	// Fallback in case this ship has no mass (WTF?)
			}

			TakeDamage(BaseDamage * CRASH_DAMAGE_SCALE * DamageScale,
				FDamageEvent(TSubclassOf<UDamageType>(UCrashDamage::StaticClass())), 
				Other->GetInstigatorController(), 
				Other);
		}

		// Drift off
		DriftVelocity = Mesh->GetPhysicsLinearVelocity();
		DriftVelocity.Z = 0;
		Speed = 0.f;
	}
}

float AShipPawn::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(HealthComponent && Role == ROLE_Authority)
	{
		// Only server may damage sth
		return HealthComponent->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		// No Health -> No Damage
		return 0.f;
	}
}

void AShipPawn::OnDestroy(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	MulticastOnDestroy(DamageEvent, EventInstigator, DamageCauser);
}

void AShipPawn::MulticastOnDestroy_Implementation(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	this->IsDying = true;

	if(OnDestroyEvent.IsBound())
	{
		// Call listeners for this event (Probably a blueprint script)
		OnDestroyEvent.Broadcast(DamageEvent.DamageTypeClass.GetDefaultObject(), EventInstigator, DamageCauser);
	}
	else if(Role == ENetRole::ROLE_Authority)
	{
		// No listeners? What a pity, just delete this actor!
		this->Destroy();
	}

	if(Role == ENetRole::ROLE_Authority)
	{
		// Tell the game about this incident
		GetWorld()->GetAuthGameMode<AMothershipGameMode>()->OnKill(EventInstigator, this->Controller, this);
	}
}

void AShipPawn::SetThrottleControl(float Throttle)
{
	ThrottleControl = Throttle;
}

inline float AShipPawn::GetThrottleControl() const
{
	return ThrottleControl;
}

void AShipPawn::SetDirectionControl(float Direction)
{
	DirectionControl = Direction;
}

inline float AShipPawn::GetDirectionControl() const
{
	return DirectionControl;
}

void AShipPawn::MovementTick(float DeltaSeconds)
{
	// The Mesh component controls the physical direction of Travel
	if(Mesh)
	{
		RotationTick(DeltaSeconds);

		// Linear movement
		if(ThrottleControl > 0.f)
		{
			Speed = FMath::Min(Speed + ForwardThrust * DeltaSeconds, MaxSpeed);
		}
		else if(ThrottleControl < 0.f)
		{
			Speed = FMath::Max(Speed - ReverseThrust * DeltaSeconds, -MaxSpeed);
		}
		else if(Speed != 0)
		{
			Speed = DampenFloat(Speed, Friction * DeltaSeconds);
		}

		// Ship always flies in the direciton it's facing (because... why not? We're arcade!)
		FVector Forward = Mesh->GetForwardVector();
		Forward.Z = 0.f;	// Safety feature... If ship gets hit it won't drift of in the Z axis
		Mesh->SetPhysicsLinearVelocity(Forward * Speed + DriftVelocity);

		// If the ship hits another object it might spin out of control -> dampen
		StabilizerTick(DeltaSeconds);
		
		// Adjust the direction the camera is facing
		if(CameraArm)
		{
			FRotator Rotator = Mesh->GetRelativeTransform().GetRotation().Rotator();
			Rotator.Roll = 0.f;
			Rotator.Pitch = CameraPitch;
			CameraArm->SetRelativeRotation(Rotator);
		}
	}
}

void AShipPawn::RotationTick(float DeltaSeconds)
{
	if(RollCurve && TurnRateCurve && Mesh)
	{
		FRotator Rotator = Mesh->GetRelativeTransform().GetRotation().Rotator();

		float SpeedFraction = FMath::Abs(Speed) / MaxSpeed;
		float MaxRoll = MaxRollInCorners * RollCurve->GetFloatValue(SpeedFraction);
		float CurRoll = Rotator.Roll;

		if(DirectionControl != 0.f)
		{
			// User steers -> Roll into the corner
			float Factor = 1.f;
			if(Speed < 0)
			{
				Factor = -1.f;
			}
			CurRoll += DirectionControl * RollRate * DeltaSeconds * Factor;
		}
		else if(CurRoll != 0.f)
		{
			// User stopped steering, bring the baby back into straight flight!
			CurRoll = DampenFloat(CurRoll, RollRate * DeltaSeconds);
		}

		// Cap Rolling at the maximum allowed values 
		CurRoll = FMath::Sign(CurRoll) * FMath::Min(MaxRoll, FMath::Abs(CurRoll));

		Rotator.Roll = CurRoll;

		// How much of the current maximum Roll are we rolling?
		float RollFraction = MaxRoll > 0.f ? CurRoll / MaxRoll : 0.f;

		if(Speed < 0)
		{
			RollFraction *= -1.f;
		}

		// Actually turn the ship
		Rotator.Yaw += RollFraction * TurnRateCurve->GetFloatValue(SpeedFraction) * TurnRate * DeltaSeconds;

		// 2.5D game -> We definitely can't pitch
		Rotator.Pitch = 0.f;

		Mesh->SetRelativeRotation(Rotator, true);
	}
}

void AShipPawn::StabilizerTick(float DeltaSeconds)
{
	// Stop any external rotation
	FVector AngularVelocity = Mesh->GetPhysicsAngularVelocity();
	if(AngularVelocity != FVector::ZeroVector)
	{
		if(AngularVelocity.X != 0.f)
			AngularVelocity.X = DampenFloat(AngularVelocity.X, RotationStabilizerStrength * DeltaSeconds * 10.f);
		if(AngularVelocity.Y != 0.f)
			AngularVelocity.Y = DampenFloat(AngularVelocity.Y, RotationStabilizerStrength * DeltaSeconds * 10.f);
		if(AngularVelocity.Z != 0.f)
			AngularVelocity.Z = DampenFloat(AngularVelocity.Z, RotationStabilizerStrength * DeltaSeconds * 10.f);
	}
	Mesh->SetPhysicsAngularVelocity(AngularVelocity);

	// Stop uncontrolled linear movement
	if(DriftVelocity != FVector::ZeroVector)
	{
		if(DriftVelocity.X != 0.f)
			DriftVelocity.X = DampenFloat(DriftVelocity.X, LinearStabilizerStrength * DeltaSeconds);
		if(DriftVelocity.Y != 0.f)
			DriftVelocity.Y = DampenFloat(DriftVelocity.Y, LinearStabilizerStrength * DeltaSeconds);
		if(DriftVelocity.Z != 0.f)
			DriftVelocity.Z = DampenFloat(DriftVelocity.Z, LinearStabilizerStrength * DeltaSeconds);
	}

	// We're a 2.5D game, so no movement on Z axis allowed.
	FVector Location = Mesh->GetRelativeTransform().GetLocation();
	Location.Z = 0.f;
	Mesh->SetRelativeLocation(Location);
}

FORCEINLINE float AShipPawn::DampenFloat(float Value, float Force)
{
	float Delta = FMath::Min(Force, FMath::Abs(Value));
	return Value - FMath::Sign(Value) * Delta;
}