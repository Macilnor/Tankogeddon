// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Machinegun.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AMachinegun : public ACannon
{
	GENERATED_BODY()
	
	protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	uint8 ShotsNumber = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float ShotsInterval = 0.1f;
	
	FTimerHandle AutomaticFireTimerHandle;
	uint8 ShotsRemains = ShotsNumber;
	bool bIsFiring = false;
	
	public:
	virtual void Fire() override;
	virtual void FireSpecial() override;

	void TriggerFire();
	void TriggerFireSpecial();
};
