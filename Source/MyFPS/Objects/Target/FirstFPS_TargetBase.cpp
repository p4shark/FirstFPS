// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_TargetBase.h"

// Sets default values
AFirstFPS_TargetBase::AFirstFPS_TargetBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshTarget",false);
	StaticMesh->SetupAttachment(RootComponent);
	AllDamage =  CreateDefaultSubobject<UTextRenderComponent>("AllDamage",false);
	AllDamage->SetupAttachment(RootComponent);
	this->bReplicates = true;
}

FName AFirstFPS_TargetBase::GetNameActor()
{
	return Name;
}

void AFirstFPS_TargetBase::Hit_Implementation(int32 DamageHit)
{
	IFirstFPS_HitInterface::Hit_Implementation(DamageHit);
}

