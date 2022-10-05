// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstFPS_InteractInterface.h"
#include "GameFramework/Actor.h"
#include "FirstFPS_HandActorBase.generated.h"

UCLASS()
class MYFPS_API AFirstFPS_HandActorBase : public AActor, public IFirstFPS_InteractInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AFirstFPS_HandActorBase();

protected:
	UPROPERTY(EditAnywhere)
	FName Name;
public:
	FName GetNameActor() const;
	void SwapActorInHandAndNewActor(ACharacter* Character);
	virtual void TakeUp_Implementation(ACharacter* Character) override;
	virtual void Drop_Implementation(ACharacter* Character) override;
};
