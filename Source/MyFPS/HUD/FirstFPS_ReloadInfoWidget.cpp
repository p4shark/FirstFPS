// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_ReloadInfoWidget.h"

#include "Kismet/KismetMathLibrary.h"

void UFirstFPS_ReloadInfoWidget::SetReloadInfo(float RemaningTime, float RateTime)
{
		FTimespan Timespan = UKismetMathLibrary::FromSeconds(RemaningTime);
		InfoAboutReload->SetText(FText::Format(FTextFormat::FromString("Reloading {0}.{1}"),Timespan.GetSeconds(),Timespan.GetFractionMilli()));
		ProgressReload->SetPercent(RemaningTime/ RateTime);

}
