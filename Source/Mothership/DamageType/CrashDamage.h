// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "CrashDamage.generated.h"

/**
 * Damage type used when crashing into something
 */
UCLASS()
class MOTHERSHIP_API UCrashDamage : public UDamageType
{
	GENERATED_BODY()
	
public:
	UCrashDamage(const FObjectInitializer& PCIP);
	
};
