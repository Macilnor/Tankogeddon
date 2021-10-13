// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
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
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurretTargetPosition(const FVector& TargetPosition);

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
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
};
