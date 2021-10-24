// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "GameFramework/Pawn.h"
#include "Scorable.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageable, public IScorable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent * CannonSetupPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		class TSubclassOf<class ACannon> CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scores")
		int32 DestructionScores = 10;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UParticleSystemComponent* HitEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UAudioComponent* HitAudioEffect;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
	class UParticleSystem* DeathEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
	class USoundBase* DeathAudioEffect;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MovementSmoothness = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float RotationSmoothness = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
		float TurretRotationSmoothness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params")
	float MovementAccuracy = 50.f;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;


public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurretTargetPosition(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetTurretRotationAxis(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Turret")	
	void SetupCannon(TSubclassOf<class ACannon> InCannonClass);
	
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void FireSpecial();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SwapCannons();

	UFUNCTION(BlueprintPure, Category = "Turret")
	class ACannon* GetCannon() const;

	UFUNCTION(BlueprintPure, Category = "Turret")
	FVector GetTurretForwardVector();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void TakeDamage(const FDamageData& DamageData) override;
	int32 GetScores() const override;

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	const TArray<FVector>& GetPatrollingPoints() 
	{ 
		return PatrollingPoints;
	}

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	float GetMovementAccuracy() 
	{ 
		return MovementAccuracy; 
	}
	
private:
	
	UPROPERTY()
	class ACannon* Cannon = nullptr;

	UPROPERTY()
	class ACannon* SecondaryCannon = nullptr;
	
	float CurrentMoveForwardAxis = 0.f;
	float TargetMoveForwardAxis = 0.f;
	float CurrentRotateRightAxis = 0.f;
	float TargetRotateRightAxis = 0.f;

	FVector TurretTargetPosition;
	FVector TurretTargetDirection;
	bool bIsTurretTargetSet = false;
	float TurretRotationAxis = 0.f;

};
