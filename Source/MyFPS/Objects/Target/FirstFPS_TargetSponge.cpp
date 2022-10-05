// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_TargetSponge.h"

#include <string>

#include "Math/TransformCalculus3D.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AFirstFPS_TargetSponge::AFirstFPS_TargetSponge()
{
	Name = FName(TEXT("Sponge Target"));
}

void AFirstFPS_TargetSponge::OnRep_Damage()
{
	AllDamage->SetText(FText::FromString(FString::FromInt(Damage)));
}

// Called when the game starts or when spawned
void AFirstFPS_TargetSponge::BeginPlay()
{
	Super::BeginPlay();
	AllDamage->SetText(FText::FromString(FString::FromInt(0)));
}

// Called every frame
void AFirstFPS_TargetSponge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirstFPS_TargetSponge::Hit_Implementation(int32 DamageHit)
{
	Damage+=DamageHit;
	OnRep_Damage();
}
void AFirstFPS_TargetSponge::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFirstFPS_TargetSponge, Damage);
}

