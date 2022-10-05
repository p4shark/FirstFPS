// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_HUD.h"

#include "FirstFPS_CheatWidget.h"
#include "FirstFPS_HealthWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "MyFPS/FirstFPS_Character.h"


// Sets default values
AFirstFPS_HUD::AFirstFPS_HUD()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFirstFPS_HUD::BeginPlay()
{
	//Super::BeginPlay();
	
		if(Cast<AFirstFPS_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
		{
			LCharacter = Cast<AFirstFPS_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
			GetOwningPlayerController()->OnPossessedPawnChanged.AddDynamic(this,&AFirstFPS_HUD::ChangePawn);
			if(IsValid(HealthWidgetClass))
			{
				HealthWidget = Cast<UFirstFPS_HealthWidget>(CreateWidget(GetWorld(),HealthWidgetClass));
				if(IsValid(HealthWidget))
				{
					HealthWidget->AddToViewport();
				}
			}
			if(IsValid(AmmoWidgetClass))
			{
				AmmoWidget = Cast<UFirstFPS_AmmoWidget>(CreateWidget(GetWorld(),AmmoWidgetClass));
				if(IsValid(AmmoWidget))
				{
					AmmoWidget->AddToViewport();
				}
			}
			if(IsValid(CheatSpawnWidgetClass))
			{
				CheatSpawnWidget = Cast<UFirstFPS_CheatWidget>(CreateWidget(GetWorld(),CheatSpawnWidgetClass));
			}
			if(IsValid(MapWidgetClass))
			{
				MapWidget = Cast<UFirstFPS_MapWidget>(CreateWidget(GetWorld(),MapWidgetClass));
			}
			
			RebindCharacter(LCharacter);
		}
	
}


void AFirstFPS_HUD::ChangePawn(APawn* OldPawn, APawn* NewPawn)
{
	if(Cast<AFirstFPS_Character>(NewPawn))
	{
		RebindCharacter(NewPawn);
	}
}

void AFirstFPS_HUD::CorrectHealth(int32 Health)
{
	HealthWidget->SetHealth(Health);
}

void AFirstFPS_HUD::InitializeMaxHealth(int32 MaxHealth)
{
	HealthWidget->SetMaxHealth(MaxHealth);
}

void AFirstFPS_HUD::CorrectCartrigesInClip(int32 Cartriges)
{
	AmmoWidget->SetCartrigesInClip(Cartriges);
}

void AFirstFPS_HUD::CorrectAllAmmo(int32 AllAmmo)
{
	AmmoWidget->SetAllAmmo(AllAmmo);
}

void AFirstFPS_HUD::SwitchVisibilityCheatWidget(bool bNVisibility)
{
	if(bNVisibility)
	{
		CheatSpawnWidget->AddToViewport();
		MapWidget->AddToViewport();
	}
	else
	{
		CheatSpawnWidget->RemoveFromParent();
		MapWidget->RemoveFromParent();
	}
}
void AFirstFPS_HUD::RebindCharacter(APawn* NewPawn)
{
	if(LCharacter != Cast<AFirstFPS_Character>(NewPawn))
	{
		LCharacter = Cast<AFirstFPS_Character>(NewPawn);
	}
	LCharacter->FChangeMaxHealth.AddDynamic(this, &AFirstFPS_HUD::InitializeMaxHealth);
	LCharacter->FChangedVisibilityCheatMenu.AddDynamic(this,&AFirstFPS_HUD::SwitchVisibilityCheatWidget);
	LCharacter->FChangedHealth.AddDynamic(this,&AFirstFPS_HUD::CorrectHealth);
	LCharacter->FChangedAmmoInActiveSlot.AddDynamic(this,&AFirstFPS_HUD::AFirstFPS_HUD::CorrectAllAmmo);
	LCharacter->FChangedCartridgesInClip.AddDynamic(this,&AFirstFPS_HUD::AFirstFPS_HUD::CorrectCartrigesInClip);
	
	CorrectHealth(LCharacter->GetHealth());
	InitializeMaxHealth(LCharacter->GetMaxHealth());
	CorrectAllAmmo(LCharacter->GetAmmoInActiveSlot());
	CorrectCartrigesInClip(LCharacter->GetCartridgesInActiveSlot());
}

