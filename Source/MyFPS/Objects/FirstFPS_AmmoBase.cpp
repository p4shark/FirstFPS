// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_AmmoBase.h"

#include "Weapons/FirstFPS_WeaponBase.h"


// Sets default values
AFirstFPS_AmmoBase::AFirstFPS_AmmoBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("mesh",false);
}


bool AFirstFPS_AmmoBase::CheckAmmoType()
{
	if(AFirstFPS_WeaponBase* Lweapon = Cast<AFirstFPS_WeaponBase>(FirstPersonCharacter->GetActorInPlayerHand()))
	{
		const UClass* Class = Lweapon->GetAmmunitionType();
		const UClass* Class1 = AmmunitionType;
		return (Class ==Class1);
	}
	return false;
}

// Called every frame
void AFirstFPS_AmmoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirstFPS_AmmoBase::TakeUp_Implementation(ACharacter* Character)
{
	//Super::TakeUp_Implementation(Character);
	if(Cast<AFirstFPS_Character>(Character))
	{
		FirstPersonCharacter = Cast<AFirstFPS_Character>(Character);
		FirstPersonCharacter->UpdateHUD();
		if(FirstPersonCharacter->GetAmmunition().Find(AmmunitionType))
		{
			LAmmo = *FirstPersonCharacter->GetAmmunition().Find(AmmunitionType);
			FirstPersonCharacter->RemoveItemFromAmmunition(AmmunitionType);
			FirstPersonCharacter->AddItemFromAmmunition(AmmunitionType,PackSize+LAmmo);
			if(CheckAmmoType())
			{
				FirstPersonCharacter->SetAmmoInActiveSlot(PackSize+LAmmo);
			};
			this->Destroy();
		}
		else
		{
			FirstPersonCharacter->AddItemFromAmmunition(AmmunitionType,PackSize);
			if(CheckAmmoType())
			{
				FirstPersonCharacter->SetAmmoInActiveSlot(PackSize);
			}
			this->Destroy();
		}
	}
}


