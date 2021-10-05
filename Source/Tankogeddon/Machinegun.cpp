// Fill out your copyright notice in the Description page of Project Settings.


#include "Machinegun.h"

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
	
	GetWorld()->GetTimerManager().SetTimer(AutomaticFireTimerHandle, this, &AMachinegun::TriggerFireSpecial, ShotsInterval, true);
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AMachinegun::Reload, 1 / FireRate, false);
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
	}
	else if(Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(10, 0.2f,FColor::Green, "Fire - trace");
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
	}
	else if(Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(10, 0.2f,FColor::Green, "Fire spacial - trace");
	}
}
