// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "PlasmaDamage.generated.h"

/**
 * 
 */
UCLASS()
class MOTHERSHIP_API UPlasmaDamage : public UDamageType
{
	GENERATED_BODY()
	
public:
	UPlasmaDamage(const FObjectInitializer& PCIP);
	
};
