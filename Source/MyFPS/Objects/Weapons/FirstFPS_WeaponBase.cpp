// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_WeaponBase.h"

#include "../../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/clsocket/src/StatTimer.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyFPS/FirstFPS_Character.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AFirstFPS_WeaponBase::AFirstFPS_WeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeltalMesh",false);
	bCanBeShoot = true;
}

void AFirstFPS_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

TSubclassOf<AFirstFPS_BulletBase> AFirstFPS_WeaponBase::GetAmmunitionType()
{
	return AmmunitionType;
}

int32 AFirstFPS_WeaponBase::GetCartridgesInClip()
{
	return CartridgesInClip;
}

void AFirstFPS_WeaponBase::SwitchReload()
{
	bReloadNow = !bReloadNow;
	if (!bReloadNow)
	{
		ReloadWeapon();
	}
}

void AFirstFPS_WeaponBase::ReloadWeapon()
{
	int32 LocalAmmoClip;
	if (CartridgesInClip != 0)
	{
		LocalAmmoClip = *GetAmmunition().Find(AmmunitionType) - (ClipSize - CartridgesInClip);
		if (LocalAmmoClip > 0)
		{
			CartridgesInClip = ClipSize;
			ChangeAmmoFromPlayer(LocalAmmoClip, CartridgesInClip);
		}
		else
		{
			CartridgesInClip = *GetAmmunition().Find(AmmunitionType) + CartridgesInClip;
			ChangeAmmoFromPlayer(0, CartridgesInClip);
		}
	}
	else
	{
		if ((*GetAmmunition().Find(AmmunitionType) - ClipSize) > 0)
		{
			CartridgesInClip = ClipSize;
			ChangeAmmoFromPlayer(*GetAmmunition().Find(AmmunitionType) - ClipSize, CartridgesInClip);
		}
		else
		{
			CartridgesInClip = *GetAmmunition().Find(AmmunitionType);
			ChangeAmmoFromPlayer(0, CartridgesInClip);
		}
	}
}

void AFirstFPS_WeaponBase::ChangeAmmoFromPlayer(int32 NewAmmo, int32 NewCartigesInClip)
{
	Cast<AFirstFPS_Character> (FirstPersonCharacterReference)->RemoveItemFromAmmunition(AmmunitionType);
	if (NewAmmo <= 0)
	{
		MulticastUpdateAmmunitionInfo(0);
		MulticastUpdateCartrigesInClipInfo(NewCartigesInClip);
	}
	else
	{
		Cast<AFirstFPS_Character> (FirstPersonCharacterReference)->AddItemFromAmmunition(AmmunitionType, NewAmmo);
		MulticastUpdateAmmunitionInfo(NewAmmo);
		MulticastUpdateCartrigesInClipInfo(NewCartigesInClip);
	}
}

void AFirstFPS_WeaponBase::CanBeFire()
{
	if (!GetAmmunition().Contains(AmmunitionType) && (CartridgesInClip == 0))
	{
		//Nothing
	}
	else
	{
		if (bReloadNow)
		{
			//Nothing
		}
		else
		{
			if (!CartridgesInClip && GetAmmunition().Contains(AmmunitionType))
			{
				FTimerHandle TimerReload;
				GetWorldTimerManager().SetTimer(TimerReload, this, &AFirstFPS_WeaponBase::SwitchReload, ReloadTime,
				                                false);
				SwitchReload();
			}
			else
			{
				if (CartridgesInClip > 0)
				{
					if (bCanBeShoot)
					{
						FTimerHandle TimerReload;
						GetWorldTimerManager().SetTimer(TimerReload, this, &AFirstFPS_WeaponBase::SwitchFireRate, RateFire,
						                                false);
						SwitchFireRate();
						OnFireProjectile();
						CartridgesInClip--;
						MulticastUpdateCartrigesInClipInfo(CartridgesInClip);
					}
				}
			}
		}
	}
}

void AFirstFPS_WeaponBase::SwitchFireRate()
{
	bCanBeShoot = !bCanBeShoot;
}

TMap<TSubclassOf<AFirstFPS_BulletBase>, int32> AFirstFPS_WeaponBase::GetAmmunition()
{
	return Cast<AFirstFPS_Character>(FirstPersonCharacterReference)->GetAmmunition();
}

void AFirstFPS_WeaponBase::OnFireProjectile()
{
	FVector Location;
	FRotator Rotator;
	FirstPersonCharacterReference->GetActorEyesViewPoint(Location, Rotator);
	Location = Rotator.RotateVector(ProjectileOffset);
	Location += SkeletalMesh->GetSocketLocation("Ammo");
	GetWorld()->UWorld::SpawnActor(AmmunitionType, &Location, &Rotator);
	ServerPlaySound(Location);
}

void AFirstFPS_WeaponBase::BindFire()
{
	Cast<AFirstFPS_Character>(FirstPersonCharacterReference)->FUsedItem.AddDynamic(this, &AFirstFPS_WeaponBase::CanBeFire);
}

void AFirstFPS_WeaponBase::UnBindFire()
{
	Cast<AFirstFPS_Character>(FirstPersonCharacterReference)->FUsedItem.RemoveDynamic(this, &AFirstFPS_WeaponBase::CanBeFire);
}

void AFirstFPS_WeaponBase::Reload_Implementation()
{
	if (bReloadNow)
	{
		//nothing
	}
	else
	{
		if (!GetAmmunition().Contains(AmmunitionType) || (CartridgesInClip == ClipSize))
		{
			//nothing
		}
		else
		{
			//ReloadWeapon();
			FTimerHandle TimerReload;
			GetWorldTimerManager().SetTimer(TimerReload, this, &AFirstFPS_WeaponBase::SwitchReload, ReloadTime, false);
			SwitchReload();
		}
	}
}

void AFirstFPS_WeaponBase::TakeUp_Implementation(ACharacter* Character)
{
	Super::TakeUp_Implementation(Character);
	
	if (AFirstFPS_Character* LocalCharacter = Cast<AFirstFPS_Character>(Character))
	{
		FirstPersonCharacterReference = LocalCharacter;
		//LocalCharacter->FUsedItem.AddDynamic(this, &AFirstFPS_WeaponBase::CanBeFire);
		BindFire();
		LocalCharacter->SetCartridgesInActiveSlot(CartridgesInClip);
		if (GetAmmunition().Contains(AmmunitionType))
		{
			LocalCharacter->SetAmmoInActiveSlot(*GetAmmunition().Find(AmmunitionType));
		}
		else
		{
			LocalCharacter->SetAmmoInActiveSlot(0);
		}
		//FirstPersonCharacterReference = LocalCharacter;
	}

}

void AFirstFPS_WeaponBase::Drop_Implementation(ACharacter* Character)
{
	
	Super::Drop_Implementation(Character);
	UnBindFire();
	//Cast<AFirstFPS_Character>(FirstPersonCharacterReference)->FUsedItem.RemoveDynamic(this, CanBeFire);
	Cast<AFirstFPS_Character>(FirstPersonCharacterReference)->SetAmmoInActiveSlot(-1);
	Cast<AFirstFPS_Character>(FirstPersonCharacterReference)->SetCartridgesInActiveSlot(-1);
	FirstPersonCharacterReference = nullptr;

}


void AFirstFPS_WeaponBase::MulticastPlaySound_Implementation(FVector Location)
{
	UGameplayStatics::PlaySoundAtLocation(FNavigationSystem::GetWorldFromContextObject(this), ShootSound, Location);
}

void AFirstFPS_WeaponBase::ServerPlaySound_Implementation(FVector Location)
{
	MulticastPlaySound(Location);
}

void AFirstFPS_WeaponBase::MulticastUpdateCartrigesInClipInfo_Implementation(int32 NewAmmo)
{
	Cast<AFirstFPS_Character>(FirstPersonCharacterReference)->FChangedCartridgesInClip.Broadcast(NewAmmo);
}

void AFirstFPS_WeaponBase::MulticastUpdateAmmunitionInfo_Implementation(int32 Ammo)
{
	Cast<AFirstFPS_Character>(FirstPersonCharacterReference)->FChangedAmmoInActiveSlot.Broadcast(Ammo);
}
void AFirstFPS_WeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFirstFPS_WeaponBase, FirstPersonCharacterReference);
	DOREPLIFETIME(AFirstFPS_WeaponBase, bReloadNow);
	DOREPLIFETIME(AFirstFPS_WeaponBase, CartridgesInClip);
}
