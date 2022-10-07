// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UObject/Object.h"
#include "FirstFPS_ReloadInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYFPS_API UFirstFPS_ReloadInfoWidget : public UUserWidget
{

	GENERATED_BODY()
public:
	void SetReloadInfo(float RemaningTime, float RateTime);
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* InfoAboutReload ;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* ProgressReload ;

};
