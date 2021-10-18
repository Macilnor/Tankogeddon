// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "Cannon.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tankogeddon.h"
#include "Chaos/ChaosPerfTest.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATankPawn::OnHealthChanged);
	HealthComponent->OnDie.AddDynamic(this, &ATankPawn::OnDie);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hit Effect"));
	HitEffect->SetupAttachment(BodyMesh);

	HitAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Hit Audio Effect"));
	HitAudioEffect->SetupAttachment(BodyMesh);
	
}

int32 ATankPawn::GetScores() const
{
	return DestructionScores;
}



// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(CannonClass);
}

void ATankPawn::Destroyed()
{
	Super::Destroyed();

	if (Cannon)
	{
		Cannon->Destroy();
	}

	if (SecondaryCannon)
	{
		SecondaryCannon->Destroy();
	}
}

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> InCannonClass)
{
	if (Cannon)
	{
		Cannon->Destroy();
	}

	if (InCannonClass)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Params);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ATankPawn::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if(Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::SwapCannons()
{
	Swap(Cannon, SecondaryCannon);
	if (Cannon)
	{
		Cannon->SetVisibility(true);
	}
	if (SecondaryCannon)
	{
		SecondaryCannon->SetVisibility(false);
	}
}

ACannon* ATankPawn::GetCannon() const
{
	return Cannon;
}

void ATankPawn::OnHealthChanged_Implementation(float Damage)
{
	UE_LOG(LogTankogeddon, Log, TEXT("Turret %s taked damage:%f "), *GetName(), Damage);
	HitEffect->ActivateSystem();
	HitAudioEffect->Play();
}

void ATankPawn::MoveForward(const float InAxisValue)
{
	TargetMoveForwardAxis = InAxisValue;
}

void ATankPawn::RotateRight(const float InAxisValue)
{
	TargetRotateRightAxis = InAxisValue;
}

void ATankPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}

void ATankPawn::OnDie_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathAudioEffect, GetActorLocation());
	Destroy();
}

// Called every frame
void ATankPawn::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentMoveForwardAxis = FMath::FInterpTo(CurrentMoveForwardAxis, TargetMoveForwardAxis, DeltaTime, MovementSmoothness);
	const FVector NewActorLocation = GetActorLocation() +
		(GetActorForwardVector() * MoveSpeed * CurrentMoveForwardAxis * DeltaTime);
	SetActorLocation(NewActorLocation, true);

	CurrentRotateRightAxis = FMath::FInterpTo(CurrentRotateRightAxis, TargetRotateRightAxis, DeltaTime, RotationSmoothness);
	const float Rotation = GetActorRotation().Yaw + CurrentRotateRightAxis * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0.f, Rotation, 0.f));

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TurretTargetPosition);
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Roll = CurrentRotation.Roll;
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, TurretRotationSmoothness));
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

