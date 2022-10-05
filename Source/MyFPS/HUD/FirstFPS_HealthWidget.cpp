// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_HealthWidget.h"

#include "Kismet/KismetTextLibrary.h"

void UFirstFPS_HealthWidget::SetHealth(int32 NHealth)
{
	ActualHealthText->SetText(FText::Format(FTextFormat::FromString("{0}"), NHealth));
}

void UFirstFPS_HealthWidget::SetMaxHealth(int32 NHealth)
{
	MaxHealthText->SetText(FText::Format(FTextFormat::FromString("/{0}"),NHealth));
}
