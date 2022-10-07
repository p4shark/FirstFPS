// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "FirstFPS_AmmoWidget.h"
#include "FirstFPS_MapWidget.h"
#include "FirstFPS_ReloadInfoWidget.h"
#include "GameFramework/HUD.h"
#include "MyFPS/FirstFPS_Character.h"
#include "FirstFPS_HUD.generated.h"

UCLASS()
class MYFPS_API AFirstFPS_HUD : public AHUD
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFirstFPS_HUD();
	
protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HealthWidgetClass;
	UPROPERTY(VisibleInstanceOnly)
	class UFirstFPS_HealthWidget* HealthWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> AmmoWidgetClass;
	UPROPERTY(VisibleInstanceOnly)
	class UFirstFPS_AmmoWidget* AmmoWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CheatSpawnWidgetClass;
	UPROPERTY()
	class UFirstFPS_CheatWidget* CheatSpawnWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MapWidgetClass;
	UPROPERTY()
	UFirstFPS_MapWidget* MapWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ReloadWidgetClass;
	UPROPERTY()
	UFirstFPS_ReloadInfoWidget* ReloadWidget;
	bool ReloadInViewport = false;
	
	UPROPERTY()
	AFirstFPS_Character* LCharacter;
	UFUNCTION()
	void ChangePawn(APawn* OldPawn,APawn* NewPawn);
	UFUNCTION()
	void CorrectHealth(int32 Health);
	UFUNCTION()
	void InitializeMaxHealth(int32 MaxHealth);
	UFUNCTION()
	void CorrectCartrigesInClip(int32 Cartriges);
	UFUNCTION()
	void CorrectAllAmmo(int32 AllAmmo);
	UFUNCTION()
	void SwitchVisibilityCheatWidget(bool bNVisibility);
	UFUNCTION()
	void VisibilityReloadWidget(bool bNVisibility,float RemaningTime, float RateTime);
	void RebindCharacter(APawn* NewPawn);
	void HiddenReloadInfo();

public:
};
