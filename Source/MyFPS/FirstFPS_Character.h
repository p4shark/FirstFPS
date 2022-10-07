// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstFPS_HitInterface.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Objects/FirstFPS_HandActorBase.h"
#include "FirstFPS_Character.generated.h"


class AFirstFPS_BulletBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUsedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangedVisibilityMapWidget, bool, Visibility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangedVisibilityCheatMenu, bool, Visibility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangedHealth, int32, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeMaxHealth, int32, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangedCartridgesInClip, int32, NewCartridges);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangedAmmoInActiveSlot, int32, NewCartridges);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequesterVisibilityReloadTime,bool, bVisibility);

UCLASS()
class MYFPS_API AFirstFPS_Character : public ACharacter, public IFirstFPS_HitInterface
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	UCameraComponent* FirstPersonCamera;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* HPBar;
public:
	AFirstFPS_Character();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginDestroy() override;
	UPROPERTY(ReplicatedUsing=OneRep_MaxHealth)
	int32 MaxHealth;
	UPROPERTY(ReplicatedUsing=OneRep_Health)
	int32 Health;
	UPROPERTY(ReplicatedUsing=OneRep_ActiveSlot)
	int32 ActiveSlot;
	UPROPERTY(Replicated)
	TArray<AFirstFPS_HandActorBase*> Inventory;
	bool bSpray;
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AFirstFPS_BulletBase>, int32> Ammunition;
	UPROPERTY(ReplicatedUsing=OneRep_bDead)
	bool bDead;
	UPROPERTY(ReplicatedUsing=OneRep_AmmoInActiveSlot)
	int32 AmmoInActiveSlot;
	UPROPERTY(ReplicatedUsing=OneRep_CartridgesInActiveSlot)
	int32 CartrigesInActiveSlot;
	UPROPERTY(ReplicatedUsing=OneRep_ReloadTime)
	FTimerHandle RelaodTimerActiveSlot;

	UPROPERTY(Replicated)
	bool bCanBeMovement = true;
	UPROPERTY()
	AController* ControllerRef;

	
public:


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void LookRightLeft(float axisvalue);
	void LookUpDown(float axisvalue);
	void MoveForwardBackward(float axisvalue);
	void MoveRightLeft(float axisvalue);

	
	UFUNCTION(Server,Unreliable)
	void ServerSelfHarm();
	UFUNCTION(Server,Unreliable)
	void ServerInteract();
	UFUNCTION(Server,Unreliable)
	void ServerDrop();
	UFUNCTION(Server,Unreliable)
	void ServerFire();
	UFUNCTION(Server,Unreliable)
	void ServerChangeActiveSlot(int32 NewSlot);
	UFUNCTION(Server,Unreliable)
	void ServerChangeActiveSlot1();
	UFUNCTION(Server,Unreliable)
	void ServerChangeActiveSlot2();
	UFUNCTION(Server,Unreliable)
	void ServerChangeActiveSlot3();
	UFUNCTION(Client,Unreliable)
	void MulticastUpdateHUD();
	UFUNCTION(Server,Unreliable)
	void ServerReload();
	UFUNCTION(Server,Unreliable)
	void ServerSwitchVisibilityCheatWidget();
	UFUNCTION(NetMulticast,Unreliable)
	void MulticastSwitchVisibilityCheatWidget();
	UFUNCTION(Server,Unreliable)
	void ServerSwitchVisibilityReloadWidget(bool bNVisibility);
	UFUNCTION(NetMulticast,Unreliable)
	void MulticastSwitchVisibilityReloadWidget(bool bNVisibility);
	UFUNCTION(Server,Unreliable,BlueprintCallable)
	void ServerSpawnActor(TSubclassOf<AActor> ClassObject);
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(FName MapName);
	UFUNCTION(Server,Unreliable)
	void ServerDeathPlayer();
	
	UFUNCTION()
	void RestartPlayer();
	UFUNCTION()
	void OneRep_MaxHealth();
	UFUNCTION()
	void OneRep_Health();
	UFUNCTION()
	void OneRep_ActiveSlot();
	UFUNCTION()
	void OneRep_bDead();
	UFUNCTION()
	void OneRep_AmmoInActiveSlot();
	UFUNCTION()
	void OneRep_CartridgesInActiveSlot();
	UFUNCTION()
	void OneRep_ReloadTime();
	
	UFUNCTION(Server,Unreliable)
	void CorrectHealth(int32 Damage);
	UFUNCTION(BlueprintCallable)
	FHitResult GetLineTrace();
	AFirstFPS_HandActorBase* GetActorInPlayerHand();
	void HideToInventory();
	void UpdateHUD();
	void UpdateTextAboutHealth(int32 NewHealth);
	virtual  void Hit_Implementation(int32 DamageHit) override;
	USkeletalMeshComponent* GetFirstPersonMesh();
	TArray<AFirstFPS_HandActorBase*> GetInventory();
	int32 GetActiveSlot();
	UFUNCTION(BlueprintCallable)
	int32 GetAmmoInActiveSlot() const;
	UFUNCTION(BlueprintCallable)
	int32 GetCartridgesInActiveSlot() const;
	UFUNCTION(BlueprintCallable)
	int32 GetMaxHealth() const;
	UFUNCTION(BlueprintCallable)
	int32 GetHealth() const;
	void SetAmmoInActiveSlot(int32 NewAmmo);
	void RemoveItemFromAmmunition(TSubclassOf<AFirstFPS_BulletBase> Key);
	void AddItemFromAmmunition(TSubclassOf<AFirstFPS_BulletBase> Key, int32 value);
	void SetCartridgesInActiveSlot(int32 NewCartriges);
	void SetInventorySlot(AFirstFPS_HandActorBase* Actor);
	UFUNCTION(Server,Unreliable)
	void ServerRelaodTimerActiveSlot(FTimerHandle NRelaodTimerActiveSlot);
	void SetRelaodTimerActiveSlot(FTimerHandle NRelaodTimerActiveSlot);
	TMap<TSubclassOf<AFirstFPS_BulletBase>, int32> GetAmmunition();

	
	UPROPERTY(BlueprintAssignable)
	FUsedItem FUsedItem;
	UPROPERTY(BlueprintAssignable)
	FChangedVisibilityMapWidget FChangedVisibilityMapWidget;
	UPROPERTY(BlueprintAssignable)
	FChangedVisibilityCheatMenu FChangedVisibilityCheatMenu;
	UPROPERTY(BlueprintAssignable)
	FChangedHealth FChangedHealth;
	UPROPERTY(BlueprintAssignable)
	FChangeMaxHealth FChangeMaxHealth;
	UPROPERTY(BlueprintAssignable)
	FChangedCartridgesInClip FChangedCartridgesInClip;
	UPROPERTY(BlueprintAssignable)
	FChangedAmmoInActiveSlot FChangedAmmoInActiveSlot;
	UPROPERTY(BlueprintAssignable)
	FRequesterVisibilityReloadTime FRequesterVisibilityReloadTime;
};
