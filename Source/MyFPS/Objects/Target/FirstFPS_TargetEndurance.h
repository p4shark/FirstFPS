// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstFPS_TargetBase.h"
#include "FirstFPS_TargetEndurance.generated.h"

UCLASS()
class MYFPS_API AFirstFPS_TargetEndurance : public AFirstFPS_TargetBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AFirstFPS_TargetEndurance();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(ReplicatedUsing=OnRep_Endurance)
	int32 Endurance;
public:
	// Called every frame
	UPROPERTY(EditAnywhere)
	UStaticMesh* DestroyedTarget;
	virtual void Hit_Implementation(int32 DamageHit) override;
	UFUNCTION()
	void OnRep_Endurance();
};

