﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
	protected:
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Mesh;
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent * ProjectileSpawnPoint;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		uint8 AmmoCapacity = 10;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 1000;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireDamage = 1;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		ECannonType Type = ECannonType::FireProjectile;
    
		FTimerHandle ReloadTimerHandle;

		uint8 Ammo = AmmoCapacity; 
		bool ReadyToFire = false;
	public:    
		ACannon();

		virtual void Fire();
		virtual void FireSpecial();

		bool IsReadyToFire() const;
	protected:
		virtual void BeginPlay() override;

		void Reload();
		void Restock(); 

};
