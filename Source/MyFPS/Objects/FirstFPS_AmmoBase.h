// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstFPS_HandActorBase.h"
#include "GameFramework/Actor.h"
#include "MyFPS/FirstFPS_Character.h"
#include "FirstFPS_AmmoBase.generated.h"

UCLASS()
class MYFPS_API AFirstFPS_AmmoBase : public AFirstFPS_HandActorBase
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
public:
	// Sets default values for this actor's properties
	AFirstFPS_AmmoBase();

protected:
	UPROPERTY()
	AFirstFPS_Character* FirstPersonCharacter;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFirstFPS_BulletBase> AmmunitionType;
	UPROPERTY(EditAnywhere)
	int32 PackSize;
	int32 LAmmo;

public:
	bool CheckAmmoType();
	virtual void Tick(float DeltaTime) override;
	virtual void TakeUp_Implementation(ACharacter* Character) override;
};
