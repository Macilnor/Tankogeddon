// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "ActorPoolSubsystem.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "Tankogeddon.h"


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
		
		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* Projectile = Cast<AProjectile>(Pool->RetrieveActor(ProjectileClass, SpawnTransform));
		if (Projectile)
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();
		}
	}
	else if(Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire - trace");

		FHitResult HitResult;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams (FName(TEXT("FireTrace")), true, this);
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5.f);
			if (HitResult.Actor.IsValid() && HitResult.Component.IsValid() && HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				HitResult.Actor->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
		}
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

		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* Projectile = Cast<AProjectile>(Pool->RetrieveActor(ProjectileClass, SpawnTransform));
		if (Projectile)
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();
		}
	}
	else if(Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire spacial - trace");
		FHitResult HitResult;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams (FName(TEXT("FireTrace")), true, this);
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5.f);
			if (HitResult.Actor.IsValid() && HitResult.Component.IsValid() && HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				HitResult.Actor->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
		}
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

void ACannon::Restock(int32 InAmmo)
{
	Ammo = FMath::Clamp(Ammo + InAmmo, 0, AmmoCapacity);
	UE_LOG(LogTankogeddon, Log, TEXT("AddAmmo(%d)! NumAmmo: %d"), InAmmo, Ammo);
}

void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
	Restock(AmmoCapacity);
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}
