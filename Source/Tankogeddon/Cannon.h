// Fill out your copyright notice in the Description page of Project Settings.

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
		float FireRate = 1.f;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		uint8 AmmoCapacity = 10;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides), Category = "Fire params")
		float FireRange = 1000;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides), Category = "Fire params")
		float FireDamage = 1;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		ECannonType Type = ECannonType::FireProjectile;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireProjectile", EditConditionHides), Category = "Fire params")
		TSubclassOf<class AProjectile> ProjectileClass;
    
		FTimerHandle ReloadTimerHandle;

		uint8 Ammo = 0; 
		bool ReadyToFire = false;
	public:    
		ACannon();

		virtual void Fire();
		virtual void FireSpecial();

		void Restock(uint8 InAmmo);
	
		bool IsReadyToFire() const;
	protected:
		virtual void BeginPlay() override;
		virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
		void Reload();
	
};
