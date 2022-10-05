// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstFPS_AmmoBase.h"
#include "Target/FirstFPS_TargetBase.h"
#include "Weapons/FirstFPS_WeaponBase.h"
#include "FirstFPS_LibraryForCheat.generated.h"

/**
 * Library for work with cheat widget and map widget
 */
UCLASS()
class MYFPS_API UFirstFPS_LibraryForCheat : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClass;
public:
	 static TMap<FName, TSubclassOf<AFirstFPS_WeaponBase>> GetAllWeapon() ;
	 static TMap<FName, TSubclassOf<AFirstFPS_AmmoBase>> GetAllAmmo() ;
	 static TMap<FName, TSubclassOf<AFirstFPS_TargetBase>> GetAllTarget() ;
	 static TMap<FName, TSubclassOf<AFirstFPS_HandActorBase>> GetAllOtherActor() ;
	 static TArray<FName> GetAllMap() ;
};
