// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FirstFPS_AmmoWidget.generated.h"

UCLASS()
class MYFPS_API UFirstFPS_AmmoWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetAllAmmo(int32 NAllAmmo);
	void SetCartrigesInClip(int32 CartrigesInClip);
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CartrigesInClipText ;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* AllAmmoText;

};
