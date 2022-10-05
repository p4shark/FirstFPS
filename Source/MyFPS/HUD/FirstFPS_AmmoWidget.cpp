// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_AmmoWidget.h"


void UFirstFPS_AmmoWidget::SetAllAmmo(int32 NAllAmmo)
{
	if(NAllAmmo!=-1)
	{
		AllAmmoText->SetText(FText::Format(FTextFormat::FromString("/{0}"), NAllAmmo));
	}
	else
	{
		AllAmmoText->SetText(FText::Format(FTextFormat::FromString("/-"),-1));
	}
}
void UFirstFPS_AmmoWidget::SetCartrigesInClip(int32 NCartrigesInClip)
{
	if(NCartrigesInClip!=-1)
	{
		CartrigesInClipText->SetText(FText::Format(FTextFormat::FromString("{0}"), NCartrigesInClip));
	}
	else
	{
		CartrigesInClipText->SetText(FText::Format(FTextFormat::FromString("-"),-1));

	}
}
