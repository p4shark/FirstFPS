// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "MyFPS/FirstFPS_HitInterface.h"
#include "FirstFPS_TargetBase.generated.h"

UCLASS(Abstract)
class MYFPS_API AFirstFPS_TargetBase : public AActor, public IFirstFPS_HitInterface
{
	GENERATED_BODY()
protected:
	FName Name;
public:	
	// Sets default values for this actor's properties
	AFirstFPS_TargetBase();
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* AllDamage;

	FName GetNameActor();
	virtual void Hit_Implementation(int32 DamageHit) override;
};
