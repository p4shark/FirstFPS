// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_MapWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "MyFPS/Objects/FirstFPS_LibraryForCheat.h"

bool UFirstFPS_MapWidget::Initialize()
{
	const bool ReturnValue = Super::Initialize();
	BT_ChangeLevel->OnClicked.AddDynamic(this,&UFirstFPS_MapWidget::ChangeLevel);
	return ReturnValue;
}

void UFirstFPS_MapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (auto item : UFirstFPS_LibraryForCheat::GetAllMap())
	{
		CB_Maps->AddOption(UKismetStringLibrary::Conv_NameToString(item));
	}
}

void UFirstFPS_MapWidget::ChangeLevel()
{
	int32 index = UFirstFPS_LibraryForCheat::GetAllMap().Find(UKismetStringLibrary::Conv_StringToName(CB_Maps->GetSelectedOption()));
	if(index !=-1)
	{
		if(AFirstFPS_Character* LCharacter = Cast<AFirstFPS_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			LCharacter->ChangeLevel(UKismetStringLibrary::Conv_StringToName(CB_Maps->GetSelectedOption()));
		}

	}
}
