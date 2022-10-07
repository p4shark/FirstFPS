// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyFPS/Bullets/FirstFPS_BulletBase.h"
#include "MyFPS/Objects/FirstFPS_HandActorBase.h"
#include "FirstFPS_WeaponBase.generated.h"



UCLASS()
class MYFPS_API AFirstFPS_WeaponBase : public AFirstFPS_HandActorBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
public:
	AFirstFPS_WeaponBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere)
	FVector ProjectileOffset;
	UPROPERTY(Replicated)
	ACharacter* FirstPersonCharacterReference;
	bool bCanBeShoot;
	UPROPERTY(Replicated)
	bool bReloadNow;
	UPROPERTY(EditAnywhere)
	int32 ClipSize;
	UPROPERTY(Replicated)
	int32 CartridgesInClip;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFirstFPS_BulletBase> AmmunitionType;
	UPROPERTY(EditAnywhere)
	float ReloadTime;
	UPROPERTY(EditAnywhere)
	float RateFire;
	UPROPERTY(EditAnywhere)
	USoundBase* ShootSound;
	bool bReloadInterruption = false;
	FTimerHandle TimerReloadСlip;
	UPROPERTY(Replicated)
	bool bVisibilityReloadInfo;
public:
	TSubclassOf<AFirstFPS_BulletBase> GetAmmunitionType();
	int32 GetCartridgesInClip();
	UFUNCTION()
	void SwitchReload();
	void ReloadWeapon();
	void ChangeAmmoFromPlayer(int32 NewAmmo, int32 NewCartigesInClip);
	UFUNCTION()
	void CanBeFire();
	void SwitchFireRate();
	TMap<TSubclassOf<AFirstFPS_BulletBase>, int32> GetAmmunition();
	void InterruptionReload();
	void SwitchVisibilityReloadInfo();
	
	UFUNCTION()
	void OnFireProjectile();
	void BindFire();
	void UnBindFire();
	UFUNCTION(NetMulticast,Reliable)
	void MulticastUpdateCartrigesInClipInfo(int32 NewAmmo);
	UFUNCTION(NetMulticast,Reliable)
	void MulticastUpdateAmmunitionInfo(int32 Ammo);
	UFUNCTION(Server,Unreliable)
	void ServerPlaySound(FVector Location);
	UFUNCTION(NetMulticast,Unreliable)
	void MulticastPlaySound(FVector Location);
	UFUNCTION(Server,Unreliable)
	void ServerReloadInfo(ACharacter* Character);
	UFUNCTION(NetMulticast,Unreliable)
	void MulticastReloadInfo(ACharacter* Character);
	virtual void Reload_Implementation() override;
	virtual void TakeUp_Implementation(ACharacter* Character) override;
	virtual void Drop_Implementation(ACharacter* Character) override;
};
