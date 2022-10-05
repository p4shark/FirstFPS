// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstFPS_TargetBase.h"
#include "FirstFPS_TargetSponge.generated.h"

UCLASS()
class MYFPS_API AFirstFPS_TargetSponge : public AFirstFPS_TargetBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFirstFPS_TargetSponge();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(ReplicatedUsing=OnRep_Damage)
	int32 Damage;
	UFUNCTION()
	void OnRep_Damage();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Hit_Implementation(int32 DamageHit) override;
};
