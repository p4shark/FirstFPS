// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FirstFPS_InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFirstFPS_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYFPS_API IFirstFPS_InteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeUp(ACharacter* Character);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Drop(ACharacter* Character);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reload();
};
