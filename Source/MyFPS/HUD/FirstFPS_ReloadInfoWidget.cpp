// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_ReloadInfoWidget.h"

#include "Kismet/KismetMathLibrary.h"

void UFirstFPS_ReloadInfoWidget::SetReloadInfo(FTimerHandle Timer)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(Timer)!=-1)
	{
		FTimespan Timespan = UKismetMathLibrary::FromSeconds(GetWorld()->GetTimerManager().GetTimerRemaining(Timer));
		InfoAboutReload->SetText(FText::Format(FTextFormat::FromString("Reloading {0}.{1}"),Timespan.GetSeconds(),Timespan.GetFractionMilli()));
		ProgressReload->SetPercent(GetWorld()->GetTimerManager().GetTimerRemaining(Timer) / GetWorld()->GetTimerManager().GetTimerRate(Timer));

	}
	
}
