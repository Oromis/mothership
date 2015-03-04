// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Mechanics), BlueprintType, meta=(BlueprintSpawnableComponent) )
class MOTHERSHIP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Called when the game starts
	virtual void InitializeComponent() override;

	float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser);

	/// Directly sets the health value of this component. This is quite low-level, consider calling TakeDamage().
	UFUNCTION(BlueprintCallable, Category = Health)
	void SetHealth(float Health);

	float GetHealth() const;
	
protected:
	void CheckHealth(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Health)
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Health)
	float MaxHealth = 0.f;
};
