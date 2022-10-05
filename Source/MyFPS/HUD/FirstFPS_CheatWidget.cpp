// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_CheatWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "MyFPS/Objects/FirstFPS_LibraryForCheat.h"

bool UFirstFPS_CheatWidget::Initialize()
{
	const bool ReturnValue = Super::Initialize();
	BT_SpawnWeapon->OnClicked.AddDynamic(this, &UFirstFPS_CheatWidget::SpawnWeapon);
	BT_SpawnAmmo->OnClicked.AddDynamic(this, &UFirstFPS_CheatWidget::SpawnAmmo);
	BT_SpawnOtherActors->OnClicked.AddDynamic(this, &UFirstFPS_CheatWidget::SpawnOtherActors);
	BT_SpawnTarget->OnClicked.AddDynamic(this, &UFirstFPS_CheatWidget::SpawnTarget);
	return ReturnValue;
}

void UFirstFPS_CheatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(bFirstStart)
	{
		TArray<FName> Keys;

		 UFirstFPS_LibraryForCheat::GetAllWeapon().GetKeys(Keys);
		 for (auto item : Keys)
		 {
		 	CB_Weapon->AddOption(UKismetStringLibrary::Conv_NameToString(item));
		 }
		 UFirstFPS_LibraryForCheat::GetAllAmmo().GetKeys(Keys);
		 for (auto item : Keys)
		 {
		 	CB_Ammo->AddOption(UKismetStringLibrary::Conv_NameToString(item));
		 }
		UFirstFPS_LibraryForCheat::GetAllTarget().GetKeys(Keys);
		 for (auto item : Keys)
		 {
		 	CB_Target->AddOption(UKismetStringLibrary::Conv_NameToString(item));
		 }
		UFirstFPS_LibraryForCheat::GetAllOtherActor().GetKeys(Keys);
		 for (auto item : Keys)
		 {
		 	CB_OtherActors->AddOption(UKismetStringLibrary::Conv_NameToString(item));
		 }
		bFirstStart = false;
	}
}

void UFirstFPS_CheatWidget::SpawnWeapon()
{
	if(TSubclassOf<AFirstFPS_WeaponBase> ClassWeapon = *UFirstFPS_LibraryForCheat::GetAllWeapon().Find(UKismetStringLibrary::Conv_StringToName(CB_Weapon->GetSelectedOption())))
	{
		if(AFirstFPS_Character* LCharacter = Cast<AFirstFPS_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			LCharacter->ServerSpawnActor(ClassWeapon);
		}

	}
}

void UFirstFPS_CheatWidget::SpawnAmmo()
{
	if(TSubclassOf<AFirstFPS_AmmoBase> ClassAmmo = *UFirstFPS_LibraryForCheat::GetAllAmmo().Find(UKismetStringLibrary::Conv_StringToName(CB_Ammo->GetSelectedOption())))
	{
		if(AFirstFPS_Character* LCharacter = Cast<AFirstFPS_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			LCharacter->ServerSpawnActor(ClassAmmo);
		}

	}
}

void UFirstFPS_CheatWidget::SpawnTarget()
{
	if(TSubclassOf<AFirstFPS_TargetBase> ClassTarget = *UFirstFPS_LibraryForCheat::GetAllTarget().Find(UKismetStringLibrary::Conv_StringToName(CB_Target->GetSelectedOption())))
	{
		if(AFirstFPS_Character* LCharacter = Cast<AFirstFPS_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			LCharacter->ServerSpawnActor(ClassTarget);
		}

	}

}

void UFirstFPS_CheatWidget::SpawnOtherActors()
{
	if(TSubclassOf<AFirstFPS_HandActorBase> ClassOtherActors = *UFirstFPS_LibraryForCheat::GetAllOtherActor().Find(UKismetStringLibrary::Conv_StringToName(CB_OtherActors->GetSelectedOption())))
	{
		if(AFirstFPS_Character* LCharacter = Cast<AFirstFPS_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			LCharacter->ServerSpawnActor(ClassOtherActors);
		}

	}
}
