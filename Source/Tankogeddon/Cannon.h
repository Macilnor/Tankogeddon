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

		UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UParticleSystemComponent* ShootEffect;

		UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UAudioComponent* AudioEffect;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1.f;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		int32 AmmoCapacity = 10;
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides), Category = "Fire params")
		float FireRange = 1000;
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireTrace", EditConditionHides), Category = "Fire params")
		float FireDamage = 1;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		ECannonType Type = ECannonType::FireProjectile;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireProjectile", EditConditionHides), Category = "Fire params")
		TSubclassOf<class AProjectile> ProjectileClass;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class UForceFeedbackEffect* ShootForceEffect;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<class UCameraShakeBase> ShootShake;
    
		FTimerHandle ReloadTimerHandle;

		int32 Ammo = 0; 
		bool ReadyToFire = false;
	public:    
		ACannon();

		virtual void Fire();
		virtual void FireSpecial();

		void SetVisibility(bool bIsVisible);

		void Restock(int32 InAmmo);
	
		bool IsReadyToFire() const;
		UFUNCTION(BlueprintPure, Category = "Ammo")
		float GetAmmoState();
		UFUNCTION(BlueprintPure, Category = "Ammo")
		float GetAmmo();
	protected:
		virtual void BeginPlay() override;
		virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
		void Reload();
	
};
