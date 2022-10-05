// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_GameMode.h"

#include "GameFramework/Character.h"

void AFirstFPS_GameMode::BeginPlay()
{
	DiedPlayer.AddDynamic(this, &AFirstFPS_GameMode::PlayerDie);
}

void AFirstFPS_GameMode::PlayerDie(ACharacter* Character)
{
	if(IsValid(Character))
	{
		if(IsValid(Character->GetController()))
		{
			RestartPlayer(Character->GetController());
		}
	}
}
