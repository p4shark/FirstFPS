// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FirstFPS_BulletBase.generated.h"

UCLASS()
class MYFPS_API AFirstFPS_BulletBase : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BulletMesh;
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
public:
	// Sets default values for this actor's properties
	AFirstFPS_BulletBase();

protected:
	UPROPERTY(EditAnywhere)
	int32 Damage;
public:
	// Called every frame
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
