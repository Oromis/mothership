// Fill out your copyright notice in the Description page of Project Settings.

#include "Mothership.h"
#include "CrashDamage.h"

UCrashDamage::UCrashDamage(const FObjectInitializer& PCIP) : Super(PCIP)
{
	bCausedByWorld = false;
}