// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

#include "Components/ArrowComponent.h"


ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * SceneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneCpm;
    
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{
	if(!ReadyToFire || Ammo < 1)
	{
		return;    
	}
	Ammo--;
	ReadyToFire = false;
    
	if(Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire - projectile");
	}
	else if(Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire - trace");
	}
    
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::FireSpecial()
{
	if(!ReadyToFire || Ammo < 1)
	{
		return;    
	}
	Ammo--;
	ReadyToFire = false;
    
	if(Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire special - projectile");
	}
	else if(Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire spacial - trace");
	}
    
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

bool ACannon::IsReadyToFire() const
{
	return ReadyToFire;
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

void ACannon::Restock()
{
	Ammo = AmmoCapacity;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}
