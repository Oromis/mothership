// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Destructible.h"
#include "ShipPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDestroyDelegate, UDamageType*, DamageType, AController*, DamageInstigator, AActor*, DamageCauser);

class UHealthComponent;
class UWeaponComponent;

UCLASS()
class MOTHERSHIP_API AShipPawn : public APawn, public IDestructible
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	AShipPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void ReceiveHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, 
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,	const FHitResult & Hit) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, 
		AActor * DamageCauser) override;

	void SetThrottleControl(float Throttle);
	float GetThrottleControl() const;

	void SetDirectionControl(float Direction);
	float GetDirectionControl() const;

	/// [Server] Called when the ship dies
	virtual void OnDestroy(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnDestroy(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	void MulticastOnDestroy_Implementation(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(BlueprintAssignable, Category = Mechanics)
	FOnDestroyDelegate OnDestroyEvent;

	virtual void SetFiring(bool Firing);

protected:

	void MovementTick(float DeltaSeconds);
	void RotationTick(float DeltaSeconds);
	void StabilizerTick(float DeltaSeconds);
	void WeaponsTick(float DeltaSeconds);

	float DampenFloat(float Value, float Force);

	bool CanBeHitBy(AActor* Other);

	// -------------------------------------------------------------------------------------
	// Components
	// -------------------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Model, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Mechanics, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	/// Curve defining the roll behavior by current speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	UCurveFloat* RollCurve;

	/// Curve modifying the turn rate by current speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	UCurveFloat* TurnRateCurve;

	// -------------------------------------------------------------------------------------
	// Status
	// -------------------------------------------------------------------------------------

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated, Category = Status)
	float Speed;

	/// When the ship hits sth or is hit by something, this is set to add some external drift
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated, Category = Status)
	FVector DriftVelocity;

	bool IsDying = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Weapons)
	bool Firing = false;

	// -------------------------------------------------------------------------------------
	// Controls
	// -------------------------------------------------------------------------------------

	/// Forward / Reverse thrust control. > 0 is forward, < 0 is backward.
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated, Category = Control)
	float ThrottleControl;

	/// Steering control. > 0 is right, < 0 is left
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated, Category = Control)
	float DirectionControl;

	// -------------------------------------------------------------------------------------
	// Guns (yay, finally the interestimg part!)
	// -------------------------------------------------------------------------------------

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapons)
	TArray<UWeaponComponent*> PrimaryWeapons;

	// -------------------------------------------------------------------------------------
	// Ship Attributes
	// -------------------------------------------------------------------------------------

	/// Acceleration in cm/s^2
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float ForwardThrust;

	/// Deceleration in cm/s^2
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float ReverseThrust;

	/// Default slowdown when doing nothing, in cm/s^2
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float Friction;

	/// Rate of turn in °/s
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float TurnRate;

	/// Maximum angle in degrees the ship rolls in corners
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float MaxRollInCorners;

	/// The speed the ship's roll value changes, in 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float RollRate;

	/// If the ship spins out of control, how quickly it stablilizes itself
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float RotationStabilizerStrength;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float LinearStabilizerStrength;

	/// Maximum ship speed in cm/s
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float MaxSpeed;

	// -------------------------------------------------------------------------------------
	// Camera
	// -------------------------------------------------------------------------------------

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	float CameraPitch;
};
