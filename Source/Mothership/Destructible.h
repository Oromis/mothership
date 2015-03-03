#pragma once

#include "Destructible.generated.h"

UINTERFACE(Blueprintable)
class UDestructible : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
* Interface stating that this actor can be destroyed (in a gameplay sense). Implementing classes can do anything in the OnDestroy() method, 
* suggested actions are e.g. displaying an explosion and / or calling the UE4 Delete() function.
*/
class IDestructible
{
	GENERATED_IINTERFACE_BODY()

	/**
	* Called when the Actor is destroyed
	* @param DamagaEvent The DamageEvent that caused the object to die. Contains the DamageType
	* @param EventInstigator WHO caused the damage (Player / AI / World)
	* @param DamageCauser WHAT caused the damage (Missile, Rock I flew against, ...)
	*/
	virtual void OnDestroy(const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};