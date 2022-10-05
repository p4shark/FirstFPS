// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "FirstFPS_CheatWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYFPS_API UFirstFPS_CheatWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	bool bFirstStart = true;
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void SpawnWeapon();
	UFUNCTION()
	void SpawnAmmo();
	UFUNCTION()
	void SpawnTarget();
	UFUNCTION()
	void SpawnOtherActors();
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BT_SpawnWeapon;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BT_SpawnAmmo;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BT_SpawnTarget;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BT_SpawnOtherActors;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* CB_Weapon;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* CB_Ammo;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* CB_Target;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* CB_OtherActors;

};
