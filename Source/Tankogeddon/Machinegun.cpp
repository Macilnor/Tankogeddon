// Fill out your copyright notice in the Description page of Project Settings.


#include "Machinegun.h"

#include "ActorPoolSubsystem.h"
#include "Damageable.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"

void AMachinegun::Fire()
{
	if(!ReadyToFire || Ammo < 1 || bIsFiring)
	{
		return;    
	}
	Ammo--;
	ReadyToFire = false;
	bIsFiring = true;

	if (Ammo < 1)
	{
		GEngine->AddOnScreenDebugMessage(10, 0.5f,FColor::Green, "Out of ammo");
	}
	
	GetWorld()->GetTimerManager().SetTimer(AutomaticFireTimerHandle, this, &AMachinegun::TriggerFire, ShotsInterval, true);
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AMachinegun::Reload, 1 / FireRate, false);
}

void AMachinegun::FireSpecial()
{
	if(!ReadyToFire || Ammo < 1 || bIsFiring)
	{
		return;    
	}
	Ammo--;
	ReadyToFire = false;
	bIsFiring = true;
	
	GetWorld()->GetTimerManager().SetTimer(AutomaticFireTimerHandle, this, &AMachinegun::TriggerFireSpecial, ShotsInterval * 2, true);
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AMachinegun::Reload, (1 / FireRate) * 2, false);
}

void AMachinegun::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimerHandle);
}

void AMachinegun::TriggerFire()
{
	if (ShotsRemains < 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimerHandle);
		ShotsRemains = ShotsNumber;
		bIsFiring = false;
	}
	ShotsRemains--;
	if(Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 0.2f,FColor::Green, "Fire - projectile.");
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
		GEngine->AddOnScreenDebugMessage(10, 0.2f,FColor::Green, "Fire - trace");
		FHitResult HitResult;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams (FName(TEXT("FireTrace")), true, this);
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			if (HitResult.Actor == GetInstigator())
			{
				return;
			}
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5.f);
			if (HitResult.Actor.IsValid() && HitResult.Component.IsValid() && HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				HitResult.Actor->Destroy();
			}
			else if (IDamageable* Damageable = Cast<IDamageable>(HitResult.Actor))
			{
				FDamageData DamageData;
				DamageData.DamageValue = FireDamage;
				DamageData.Instigator = GetInstigator();
				DamageData.DamageMaker = this;
				Damageable->TakeDamage(DamageData);
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
		}
	}
}

void AMachinegun::TriggerFireSpecial()
{
	if (ShotsRemains < 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimerHandle);
		ShotsRemains = ShotsNumber;
		bIsFiring = false;

	}
	ShotsRemains--;
	if(Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 0.2f,FColor::Green, "Fire special - projectile");
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
		GEngine->AddOnScreenDebugMessage(10, 0.2f,FColor::Green, "Fire spacial - trace");
		FHitResult HitResult;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams (FName(TEXT("FireTrace")), true, this);
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			if (HitResult.Actor == GetInstigator())
			{
				return;
			}
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5.f);
			if (HitResult.Actor.IsValid() && HitResult.Component.IsValid() && HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				HitResult.Actor->Destroy();
			}
			else if (IDamageable* Damageable = Cast<IDamageable>(HitResult.Actor))
			{
				FDamageData DamageData;
				DamageData.DamageValue = FireDamage * 0.75f;
				DamageData.Instigator = GetInstigator();
				DamageData.DamageMaker = this;
				Damageable->TakeDamage(DamageData);
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
		}
	}
}
