#include "Mothership.h"
#include "HealthComponent.h"
#include "Destructible.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	
}

float UHealthComponent::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	// TODO shields, respect damage type, ...
	SetHealth(Health - DamageAmount);
	CheckHealth(DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

void UHealthComponent::SetHealth(float Health)
{
	this->Health = FMath::Clamp(Health, 0.f, MaxHealth);
}

FORCEINLINE float UHealthComponent::GetHealth() const
{
	return Health;
}

FORCEINLINE void UHealthComponent::CheckHealth(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(Health <= 0.f)
	{
		// Actually dead ... :)
		AActor* Owner = this->GetOwner();
		if(Owner)
		{
			if(IDestructible* Destructible = Cast<IDestructible>(Owner))
			{
				Destructible->OnDestroy(DamageEvent, EventInstigator, DamageCauser);
			}
		}
	}
}