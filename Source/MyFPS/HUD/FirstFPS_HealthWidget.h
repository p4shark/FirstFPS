// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FirstFPS_HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYFPS_API UFirstFPS_HealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetHealth(int32 NHealth);
	void SetMaxHealth(int32 NHealth);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ActualHealthText ;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* MaxHealthText ;
	
};
