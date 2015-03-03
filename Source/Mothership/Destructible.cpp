#include "Mothership.h"
#include "Destructible.h"

UDestructible::UDestructible(const FObjectInitializer& PCIP) : Super(PCIP)
{

}

void IDestructible::OnDestroy(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
}