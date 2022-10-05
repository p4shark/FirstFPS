// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_HandActorBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "MyFPS/FirstFPS_Character.h"
#include "Weapons/FirstFPS_WeaponBase.h"

class AFirstFPS_Character;
// Sets default values
AFirstFPS_HandActorBase::AFirstFPS_HandActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->SetReplicates(true);
	this->SetReplicateMovement(true);
}

FName AFirstFPS_HandActorBase::GetNameActor() const
{
	return Name;
}

void AFirstFPS_HandActorBase::SwapActorInHandAndNewActor(ACharacter* Character)
{

	if(AFirstFPS_Character* LCharacter = Cast<AFirstFPS_Character>(Character))
	{
		if(AFirstFPS_WeaponBase* WeaponActor = Cast<AFirstFPS_WeaponBase>(LCharacter->GetActorInPlayerHand()))
		{
			WeaponActor->UnBindFire();
		}
		Cast<AFirstFPS_Character>(Character)->SetAmmoInActiveSlot(-1);
		Cast<AFirstFPS_Character>(Character)->SetCartridgesInActiveSlot(-1);

		LCharacter->GetActorInPlayerHand()->DetachFromActor(FDetachmentTransformRules(FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepRelative,EDetachmentRule::KeepWorld,true)));
		LCharacter->GetActorInPlayerHand()->SetActorLocation(GetActorLocation(),false,false);
		LCharacter->GetActorInPlayerHand()->SetActorEnableCollision(true);
		this->SetActorEnableCollision(false);
		LCharacter->SetInventorySlot( this);
		if(HasAuthority())
		{
			AttachToComponent(LCharacter->GetFirstPersonMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,true),"GripPoint");
		}
		else
		{
			AttachToComponent(Character->GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,true),"hand_r");
		}
	}
}

void AFirstFPS_HandActorBase::TakeUp_Implementation(ACharacter* Character)
{
	if(AFirstFPS_Character* LocalCharacter = Cast<AFirstFPS_Character>(Character))
	{
		if(IsValid(LocalCharacter->GetActorInPlayerHand()))
		{
			SwapActorInHandAndNewActor(LocalCharacter);
		}
		else
		{
			if(HasAuthority())
			{
				AttachToComponent(LocalCharacter->GetFirstPersonMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,true),"GripPoint");
			}
			else
			{
				AttachToComponent(LocalCharacter->GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,true),"hand_r");
			}
			this->SetActorEnableCollision(false);
			LocalCharacter->SetInventorySlot(this);
		}
	}
}

void AFirstFPS_HandActorBase::Drop_Implementation(ACharacter* Character)
{
	
	if(AFirstFPS_Character* LocalCharacter = Cast<AFirstFPS_Character>(Character))
	{
		FVector NewTransformLocation = LocalCharacter->GetActorForwardVector();
		UKismetMathLibrary::Vector_Normalize(NewTransformLocation,0.0001);
		NewTransformLocation = UKismetMathLibrary::Multiply_VectorFloat(NewTransformLocation,100.0f)+LocalCharacter->GetActorLocation();
		DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepRelative,EDetachmentRule::KeepWorld,true));
		SetActorLocation(NewTransformLocation,false,nullptr,ETeleportType::None);
		this->SetActorEnableCollision(true);
		LocalCharacter->SetInventorySlot(nullptr);
	}
}
