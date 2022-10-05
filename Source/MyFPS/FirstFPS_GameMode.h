// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Serialization/PropertyLocalizationDataGathering.h"
#include "UObject/Object.h"
#include "FirstFPS_GameMode.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiedPlayer, ACharacter*, Character);
UCLASS()
class MYFPS_API AFirstFPS_GameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void PlayerDie(ACharacter* Character);
public:
	FDiedPlayer DiedPlayer;
};
