// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstFPS_BulletBase.h"

#include "MyFPS/FirstFPS_HitInterface.h"


// Sets default values
AFirstFPS_BulletBase::AFirstFPS_BulletBase()
{
	this->bReplicates = true;
	this->bAlwaysRelevant = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(8.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Projectile"),true);
	RootComponent = SphereComponent;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->bShouldBounce =true;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed =3000.0f;
	ProjectileMovement->MaxSpeed =3000.0f;
	ProjectileMovement->Velocity = FVector(1,0,0)*ProjectileMovement->InitialSpeed;
	
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetCollisionProfileName(TEXT("NoCollision"),true);
	BulletMesh->SetupAttachment(RootComponent);
	Damage = 10;
	
}

void AFirstFPS_BulletBase::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if(HasAuthority() && Other->Implements<UFirstFPS_HitInterface>())
	{
		IFirstFPS_HitInterface::Execute_Hit(Other,Damage);
	}
	this->Destroy();
}

