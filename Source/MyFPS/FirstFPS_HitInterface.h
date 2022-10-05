// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FirstFPS_HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFirstFPS_HitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYFPS_API IFirstFPS_HitInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Hit(int32 DamageHit);
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
