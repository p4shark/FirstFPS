// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_Character.h"

#include "FirstFPS_GameMode.h"
#include "K2Node_SpawnActorFromClass.h"
#include "MovieSceneSequenceID.h"
#include "SAdvancedRotationInputBox.h"
#include "AI/NavigationSystemBase.h"
#include "Components/CapsuleComponent.h"
//#include "Kismet/BlueprintMapLibrary.h"
//#include "Kismet/GameplayStatics.h"


#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Objects/Weapons/FirstFPS_WeaponBase.h"


// Sets default values
AFirstFPS_Character::AFirstFPS_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	this->SetReplicates(true);
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("Camera", false);
	HPBar = CreateDefaultSubobject<UTextRenderComponent>("HPBar", false);
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FPMesh", false);
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	HPBar->SetupAttachment(RootComponent);
	FirstPersonCamera->SetupAttachment(RootComponent);
	bCanBeMovement = true;
}

// Called when the game starts or when spawned
void AFirstFPS_Character::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		MaxHealth = UKismetMathLibrary::RandomIntegerInRange(50, 100);
		Health = UKismetMathLibrary::RandomIntegerInRange(50, MaxHealth);
		AmmoInActiveSlot = -1;
		CartrigesInActiveSlot = -1;
	}
	OneRep_CartridgesInActiveSlot();
	OneRep_MaxHealth();
	OneRep_Health();
	OneRep_AmmoInActiveSlot();
	Inventory.Init(nullptr, 3);
}

void AFirstFPS_Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bSpray)
	{
		FUsedItem.Broadcast();
	}
}

void AFirstFPS_Character::BeginDestroy()
{
	Super::BeginDestroy();
	if (AFirstFPS_GameMode* GameMode = Cast<AFirstFPS_GameMode>(
		UGameplayStatics::GetGameMode(FNavigationSystem::GetWorldFromContextObject(this))))
	{
		GameMode->DiedPlayer.Broadcast(this);
	}
}

void AFirstFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &AFirstFPS_Character::LookRightLeft);
	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &AFirstFPS_Character::LookUpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFirstFPS_Character::MoveForwardBackward);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &AFirstFPS_Character::MoveRightLeft);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("interract"), IE_Pressed, this, &AFirstFPS_Character::ServerInteract);
	PlayerInputComponent->BindAction(TEXT("Drop"), IE_Pressed, this, &AFirstFPS_Character::ServerDrop);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AFirstFPS_Character::ServerReload);
	PlayerInputComponent->BindAction(TEXT("PrimaryAction"), IE_Pressed, this, &AFirstFPS_Character::ServerFire);
	PlayerInputComponent->BindAction(TEXT("PrimaryAction"), IE_Released, this, &AFirstFPS_Character::ServerFire);
	PlayerInputComponent->BindAction(TEXT("QuickInventory1"), IE_Pressed, this,
	                                 &AFirstFPS_Character::ServerChangeActiveSlot1);
	PlayerInputComponent->BindAction(TEXT("QuickInventory2"), IE_Pressed, this,
	                                 &AFirstFPS_Character::ServerChangeActiveSlot2);
	PlayerInputComponent->BindAction(TEXT("QuickInventory3"), IE_Pressed, this,
	                                 &AFirstFPS_Character::ServerChangeActiveSlot3);
	PlayerInputComponent->BindAction(TEXT("CheatMenu"), IE_Pressed, this, &AFirstFPS_Character::ServerSwitchVisibilityCheatWidget);
}

void AFirstFPS_Character::LookRightLeft(float axisvalue)
{
	if(bCanBeMovement)
		this->AddControllerYawInput(axisvalue);
}

void AFirstFPS_Character::LookUpDown(float axisvalue)
{
	if(bCanBeMovement)
		this->AddControllerPitchInput(axisvalue);
}

void AFirstFPS_Character::MoveForwardBackward(float axisvalue)
{
	this->AddMovementInput(this->GetActorForwardVector() * axisvalue);
}

void AFirstFPS_Character::MoveRightLeft(float axisvalue)
{
	this->AddMovementInput(this->GetActorRightVector() * axisvalue);
}

void AFirstFPS_Character::ServerChangeActiveSlot1_Implementation()
{
	ServerChangeActiveSlot(0);
}

void AFirstFPS_Character::ServerChangeActiveSlot2_Implementation()
{
	ServerChangeActiveSlot(1);
}

void AFirstFPS_Character::ServerChangeActiveSlot3_Implementation()
{
	ServerChangeActiveSlot(2);
}

void AFirstFPS_Character::ServerFire_Implementation()
{
	bSpray = !bSpray;
}


void AFirstFPS_Character::ServerSelfHarm_Implementation()
{
	CorrectHealth(10);
}

void AFirstFPS_Character::ServerInteract_Implementation()
{
	FHitResult InteractHitResult = GetLineTrace();
	if (InteractHitResult.IsValidBlockingHit())
	{
		AActor* HitActor = InteractHitResult.GetActor();
		if (HitActor->GetClass()->ImplementsInterface(UFirstFPS_InteractInterface::StaticClass()))
		{
			IFirstFPS_InteractInterface::Execute_TakeUp(HitActor, this);
		}
	}
}

void AFirstFPS_Character::ServerDrop_Implementation()
{
	if (IsValid(Inventory[ActiveSlot]))
	{
		if (Inventory[ActiveSlot]->GetClass()->ImplementsInterface(UFirstFPS_InteractInterface::StaticClass()))
		{	
			IFirstFPS_InteractInterface::Execute_Drop(Inventory[ActiveSlot], this);
		}
	}
}

void AFirstFPS_Character::ServerChangeActiveSlot_Implementation(int32 NewSlot)
{
	if (ActiveSlot != NewSlot)
	{
		HideToInventory();
		ActiveSlot = NewSlot;
		OneRep_ActiveSlot();
	}
}

void AFirstFPS_Character::MulticastUpdateHUD_Implementation()
{
	UpdateHUD();
}

void AFirstFPS_Character::ServerReload_Implementation()
{
	if(IsValid(GetActorInPlayerHand()))
	{
		if (GetActorInPlayerHand()->Implements<UFirstFPS_InteractInterface>())
		{
			IFirstFPS_InteractInterface::Execute_Reload(GetActorInPlayerHand());
		}
	}
}

void AFirstFPS_Character::ServerSwitchVisibilityCheatWidget_Implementation()
{
	bCanBeMovement = !bCanBeMovement;
	if(IsValid(GetController()))
	{
		Cast<APlayerController>(GetController())->SetShowMouseCursor(!bCanBeMovement);
		FChangedVisibilityCheatMenu.Broadcast(!bCanBeMovement);
	}
	MulticastSwitchVisibilityCheatWidget();
}

void AFirstFPS_Character::ServerSpawnActor_Implementation(TSubclassOf<AActor> ClassObject)
{
	
	FVector SpawnTransformLocation;
	SpawnTransformLocation = GetActorForwardVector();
	UKismetMathLibrary::Vector_Normalize(SpawnTransformLocation,0.0001);
	SpawnTransformLocation = UKismetMathLibrary::Multiply_VectorFloat(SpawnTransformLocation,100.0f);
	SpawnTransformLocation+=GetActorLocation();
	SpawnTransformLocation.Z =50.0f;
	FRotator Rotator = FRotator(0.0f,0.0f,0.0f);
 	GetWorld()->UWorld::SpawnActor(ClassObject,&SpawnTransformLocation,&Rotator);

}


void AFirstFPS_Character::MulticastSwitchVisibilityCheatWidget_Implementation()
{
	if(IsValid(GetController()))
	{
		Cast<APlayerController>(GetController())->SetShowMouseCursor(!bCanBeMovement);
		FChangedVisibilityCheatMenu.Broadcast(!bCanBeMovement);
	}
}

void AFirstFPS_Character::ServerSwitchVisibilityReloadWidget_Implementation(bool bNVisibility,FTimerHandle TimerReload)
{
	FRequesterVisibilityReloadTime.Broadcast(bNVisibility,GetWorldTimerManager().GetTimerRemaining(TimerReload),GetWorldTimerManager().GetTimerRate(TimerReload));
	MulticastSwitchVisibilityReloadWidget(bNVisibility,GetWorldTimerManager().GetTimerRemaining(TimerReload),GetWorldTimerManager().GetTimerRate(TimerReload));
}

void AFirstFPS_Character::MulticastSwitchVisibilityReloadWidget_Implementation(bool bNVisibility,float RemaningTime, float RateTime)
{
	
	FRequesterVisibilityReloadTime.Broadcast(bNVisibility,RemaningTime,RateTime);
}

void AFirstFPS_Character::ChangeLevel(FName MapName)
{
	UGameplayStatics::OpenLevel(FNavigationSystem::GetWorldFromContextObject(this),MapName);
}

void AFirstFPS_Character::ServerDeathPlayer_Implementation()
{
	if(IsValid(Inventory[ActiveSlot]))
	{
		if (Inventory[ActiveSlot]->GetClass()->ImplementsInterface(UFirstFPS_InteractInterface::StaticClass()))
		{
			IFirstFPS_InteractInterface::Execute_Drop(Inventory[ActiveSlot], this);
		}
	}
	
	
	if(APlayerController* PlayerCOntroller = Cast<APlayerController>(GetController()))
	{
		
		bDead = true;
		OneRep_bDead();
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer,this,&AFirstFPS_Character::RestartPlayer,3.0f,false);
	}
}

void AFirstFPS_Character::RestartPlayer()
{
	ControllerRef = GetController();
	this->Destroy();
	if(AFirstFPS_GameMode* GameMode = Cast<AFirstFPS_GameMode>(
		UGameplayStatics::GetGameMode(FNavigationSystem::GetWorldFromContextObject(this))))
	{
		GameMode->RestartPlayer(ControllerRef);
	}
	
}

void AFirstFPS_Character::OneRep_MaxHealth()
{
	FChangeMaxHealth.Broadcast(MaxHealth);
}

void AFirstFPS_Character::OneRep_Health()
{
	HPBar->SetText(FText::Format(FTextFormat::FromString("{0}/{1}"), Health, MaxHealth));
	FChangedHealth.Broadcast(Health);
	if (Health == 0)
	{
		if (!bDead)
		{
			ServerDeathPlayer();
		}
	}
}

void AFirstFPS_Character::OneRep_ActiveSlot()
{
	if (HasAuthority())
	{
		if (IsValid(GetActorInPlayerHand()))
		{
			AFirstFPS_HandActorBase* ActorInHand = GetActorInPlayerHand();

			ActorInHand->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale,
			                               "GripPoint");
			if (AFirstFPS_WeaponBase* WeaponInHand = Cast<AFirstFPS_WeaponBase>(ActorInHand))
			{
				WeaponInHand->InterruptionReload();
				WeaponInHand->BindFire();
				const TSubclassOf<AFirstFPS_BulletBase> Key = WeaponInHand->GetAmmunitionType();

				if (const int* Value = Ammunition.Find(Key))
				{
					AmmoInActiveSlot = *Value;
					OneRep_AmmoInActiveSlot();
				}
				else
				{
					AmmoInActiveSlot = 0;
					OneRep_AmmoInActiveSlot();
				}
				CartrigesInActiveSlot = WeaponInHand->GetCartridgesInClip();
				OneRep_CartridgesInActiveSlot();
			}
			else
			{
				AmmoInActiveSlot = -1;
				OneRep_AmmoInActiveSlot();
				CartrigesInActiveSlot = -1;
				OneRep_CartridgesInActiveSlot();
			}
		}
		else
		{
			AmmoInActiveSlot = -1;
			OneRep_AmmoInActiveSlot();
			CartrigesInActiveSlot = -1;
			OneRep_CartridgesInActiveSlot();
		}
	}
}

void AFirstFPS_Character::OneRep_bDead()
{
	GetMesh()->SetSimulatePhysics(true);
	HPBar->SetHiddenInGame(true);
	GetFirstPersonMesh()->SetHiddenInGame(true);
	GetCapsuleComponent()->DestroyComponent();
	if(APlayerController* PlayerCOntroller = Cast<APlayerController>(GetController()))
	{
		DisableInput(PlayerCOntroller);
	}

}

void AFirstFPS_Character::OneRep_AmmoInActiveSlot()
{
	FChangedAmmoInActiveSlot.Broadcast(AmmoInActiveSlot);
}

void AFirstFPS_Character::OneRep_CartridgesInActiveSlot()
{
	FChangedCartridgesInClip.Broadcast(CartrigesInActiveSlot);
}

void AFirstFPS_Character::CorrectHealth_Implementation(int32 Damage)
{
	Health = UKismetMathLibrary::Max(0, Health - Damage);
	OneRep_Health();
}

FHitResult AFirstFPS_Character::GetLineTrace()
{
	FHitResult OutHit;

	FRotator CameraRotator;
	FVector StartVector, EndVector;
	StartVector = FirstPersonCamera->GetComponentLocation();
	GetController()->GetActorEyesViewPoint(EndVector, CameraRotator);
	EndVector = UKismetMathLibrary::GetForwardVector(CameraRotator);
	UKismetMathLibrary::Vector_Normalize(EndVector, 0.0001);
	EndVector = UKismetMathLibrary::Multiply_VectorFloat(EndVector, 1000.0f);
	EndVector += StartVector;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(OutHit, StartVector, EndVector,
		ECollisionChannel::ECC_GameTraceChannel3, Params);
	DrawDebugLine(GetWorld(), StartVector, EndVector, FColor::Red, false, 5.0f);
	return OutHit;
}

AFirstFPS_HandActorBase* AFirstFPS_Character::GetActorInPlayerHand()
{
	return Inventory[ActiveSlot];
}

void AFirstFPS_Character::HideToInventory()
{
	if (IsValid(GetActorInPlayerHand()))
	{
		AFirstFPS_HandActorBase* ActorInHand = GetActorInPlayerHand();
		ActorInHand->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepRelative,EDetachmentRule::KeepWorld,true));
		ActorInHand->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, -300.0f), false, nullptr,
		                              ETeleportType::None);
		if (AFirstFPS_WeaponBase* WeaponInHand = Cast<AFirstFPS_WeaponBase>(ActorInHand))
		{
			FTimerHandle ZeroTaimer;
			GetWorldTimerManager().ClearTimer(ZeroTaimer);
			WeaponInHand->UnBindFire();
			WeaponInHand->InterruptionReload();
			ServerSwitchVisibilityReloadWidget(false,ZeroTaimer);
			MulticastSwitchVisibilityReloadWidget(false,0,1);
		}
	}
}

void AFirstFPS_Character::UpdateHUD()
{
	if (IsValid(GetActorInPlayerHand()))
	{
		AFirstFPS_HandActorBase* ActorInHand = GetActorInPlayerHand();
		if (AFirstFPS_WeaponBase* WeaponInHand = Cast<AFirstFPS_WeaponBase>(ActorInHand))
		{
				FChangedCartridgesInClip.Broadcast(WeaponInHand->GetCartridgesInClip());
				FChangedAmmoInActiveSlot.Broadcast(AmmoInActiveSlot);
		}
	}
	else
	{
		FChangedCartridgesInClip.Broadcast(-1);
		FChangedAmmoInActiveSlot.Broadcast(-1);
	}
}

void AFirstFPS_Character::UpdateTextAboutHealth(int32 NewHealth)
{
	HPBar->SetText(FText::Format(FTextFormat::FromString("{0}/{1}"), NewHealth, MaxHealth));
}

void AFirstFPS_Character::Hit_Implementation(int32 DamageHit)
{
	CorrectHealth(DamageHit);
}

USkeletalMeshComponent* AFirstFPS_Character::GetFirstPersonMesh()
{
	return FirstPersonMesh;
}

TArray<AFirstFPS_HandActorBase*> AFirstFPS_Character::GetInventory()
{
	return Inventory;
}

int32 AFirstFPS_Character::GetActiveSlot()
{
	return ActiveSlot;
}

int32 AFirstFPS_Character::GetAmmoInActiveSlot() const
{
	return AmmoInActiveSlot;
}

int32 AFirstFPS_Character::GetCartridgesInActiveSlot() const
{
	return CartrigesInActiveSlot;
}

int32 AFirstFPS_Character::GetMaxHealth() const
{
	return MaxHealth;
}

int32 AFirstFPS_Character::GetHealth() const
{
	return Health;
}

void AFirstFPS_Character::SetAmmoInActiveSlot(int32 NewAmmo)
{
	AmmoInActiveSlot = NewAmmo;
	OneRep_AmmoInActiveSlot();
}

void AFirstFPS_Character::RemoveItemFromAmmunition(TSubclassOf<AFirstFPS_BulletBase> Key)
{
	Ammunition.Remove(Key);
}

void AFirstFPS_Character::AddItemFromAmmunition(TSubclassOf<AFirstFPS_BulletBase> Key, int32 value)
{
	Ammunition.Add(Key, value);
}

void AFirstFPS_Character::SetCartridgesInActiveSlot(int32 NewCartriges)
{
	CartrigesInActiveSlot = NewCartriges;
	OneRep_CartridgesInActiveSlot();
}

void AFirstFPS_Character::SetInventorySlot(AFirstFPS_HandActorBase* Actor)
{
	Inventory[ActiveSlot] = Actor;
}


TMap<TSubclassOf<AFirstFPS_BulletBase>, int32> AFirstFPS_Character::GetAmmunition()
{
	return Ammunition;
}

void AFirstFPS_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFirstFPS_Character, Health);
	DOREPLIFETIME(AFirstFPS_Character, MaxHealth);
	DOREPLIFETIME(AFirstFPS_Character, AmmoInActiveSlot);
	DOREPLIFETIME(AFirstFPS_Character, CartrigesInActiveSlot);
	DOREPLIFETIME(AFirstFPS_Character, ActiveSlot);
	DOREPLIFETIME(AFirstFPS_Character, bDead);
	DOREPLIFETIME(AFirstFPS_Character, bCanBeMovement);
	DOREPLIFETIME(AFirstFPS_Character, Inventory);
}
