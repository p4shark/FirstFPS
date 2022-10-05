// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "UObject/Object.h"
#include "FirstFPS_MapWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYFPS_API UFirstFPS_MapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void ChangeLevel();
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* BT_ChangeLevel;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* CB_Maps;
};
