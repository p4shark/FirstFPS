// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_TargetEndurance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


AFirstFPS_TargetEndurance::AFirstFPS_TargetEndurance()
{
	Name = FName(TEXT("Endurance Target"));
}

// Called when the game starts or when spawned
void AFirstFPS_TargetEndurance::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		Endurance = UKismetMathLibrary::RandomIntegerInRange(100,200);
	}
	AllDamage->SetText(FText::FromString(FString::FromInt(Endurance)));
}

void AFirstFPS_TargetEndurance::Hit_Implementation(int32 DamageHit)
{
	Endurance-=DamageHit;
	OnRep_Endurance();
}

void AFirstFPS_TargetEndurance::OnRep_Endurance()
{
	AllDamage->SetText(FText::FromString(FString::FromInt(UKismetMathLibrary::Max(0,Endurance))));
	if(UKismetMathLibrary::Max(0,Endurance)==0)
	{
		StaticMesh->SetStaticMesh(DestroyedTarget);
		StaticMesh->ResetRelativeTransform();
		AllDamage->SetHiddenInGame(true);
	}
}

void AFirstFPS_TargetEndurance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFirstFPS_TargetEndurance, Endurance);
}