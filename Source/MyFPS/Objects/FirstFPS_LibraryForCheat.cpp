// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_LibraryForCheat.h"

#include "FirstFPS_AmmoBase.h"
#include "Kismet/GameplayStatics.h"

TMap<FName, TSubclassOf<AFirstFPS_WeaponBase>> UFirstFPS_LibraryForCheat::GetAllWeapon()
{
	const UWorld* world = GEngine-> GameViewport-> GetWorld();
	TMap<FName, TSubclassOf<AFirstFPS_WeaponBase>> ResultMap;
	AFirstFPS_WeaponBase* Actor;
	TArray<AActor*> ArrayActor;
	UGameplayStatics::GetAllActorsOfClass(world, Actor->StaticClass() ,ArrayActor);
	for (auto item : ArrayActor)
	{
		if(!ResultMap.Find(Cast<AFirstFPS_WeaponBase>(item)->GetNameActor()) && Cast<AFirstFPS_WeaponBase>(item)->GetNameActor() != TEXT("None"))
		{
			ResultMap.Add(Cast<AFirstFPS_WeaponBase>(item)->GetNameActor(),item->GetClass());
		}
	}
	return ResultMap;
}

TMap<FName, TSubclassOf<AFirstFPS_AmmoBase>> UFirstFPS_LibraryForCheat::GetAllAmmo()
{
	UWorld* world = GEngine-> GameViewport-> GetWorld();
	static TMap<FName, TSubclassOf<AFirstFPS_AmmoBase>> ResultMap;
	AFirstFPS_AmmoBase* Actor;
	TArray<AActor*> ArrayActor;
	UGameplayStatics::GetAllActorsOfClass(world, Actor->StaticClass(),ArrayActor);
	for (auto item : ArrayActor)
	{
		if(!ResultMap.Find(Cast<AFirstFPS_AmmoBase>(item)->GetNameActor()) && Cast<AFirstFPS_AmmoBase>(item)->GetNameActor() != TEXT("None"))
		{
			ResultMap.Add(Cast<AFirstFPS_AmmoBase>(item)->GetNameActor(),item->GetClass());
		}
	}
	return ResultMap;
}

TMap<FName, TSubclassOf<AFirstFPS_TargetBase>> UFirstFPS_LibraryForCheat::GetAllTarget()
{
	UWorld* world = GEngine-> GameViewport-> GetWorld();
	static TMap<FName, TSubclassOf<AFirstFPS_TargetBase>> ResultMap;
	AFirstFPS_TargetBase* Actor;
	TArray<AActor*> ArrayActor;
	UGameplayStatics::GetAllActorsOfClass(world, Actor->StaticClass(),ArrayActor);
	for (auto item : ArrayActor)
	{
		if(!ResultMap.Find(Cast<AFirstFPS_TargetBase>(item)->GetNameActor()) && Cast<AFirstFPS_TargetBase>(item)->GetNameActor() != TEXT("None"))
		{
			ResultMap.Add(Cast<AFirstFPS_TargetBase>(item)->GetNameActor(),item->GetClass());
		}
	}
	return ResultMap;
}

TMap<FName, TSubclassOf<AFirstFPS_HandActorBase>> UFirstFPS_LibraryForCheat::GetAllOtherActor()
{
	UWorld* world = GEngine-> GameViewport-> GetWorld();
	static TMap<FName, TSubclassOf<AFirstFPS_HandActorBase>> ResultMap;
	AFirstFPS_HandActorBase* Actor;
	TArray<AActor*> ArrayActor;
	UGameplayStatics::GetAllActorsOfClass(world, Actor->StaticClass(),ArrayActor);
	for (auto item : ArrayActor)
	{
		if(!ResultMap.Find(Cast<AFirstFPS_HandActorBase>(item)->GetNameActor()) && Cast<AFirstFPS_HandActorBase>(item)->GetNameActor() != TEXT("None"))
		{
			if(!Cast<AFirstFPS_WeaponBase>(item) && !Cast<AFirstFPS_AmmoBase>(item) )
			{
				ResultMap.Add(Cast<AFirstFPS_HandActorBase>(item)->GetNameActor(),item->GetClass());
			}
		}
	}
	return ResultMap;
}

TArray<FName> UFirstFPS_LibraryForCheat::GetAllMap()
{
	static TArray<FName> Result;
	if(Result.Find("MapForTest")==-1)
	{
		Result.Add(TEXT("MapForTest"));
		Result.Add(TEXT("FirstPersonMap"));
	}
	return  Result;
}


