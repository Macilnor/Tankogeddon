// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/NoExportTypes.h"
#include "Tankogeddon.h"
#include "HealthComponent.h"
#include "HPBarWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
    RootComponent = BodyMesh;

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
    TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

    CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
    CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

    HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
    HitCollider->SetupAttachment(BodyMesh);
    
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
    HealthComponent->OnHealthChanged.AddDynamic(this, &ATurret::OnHealthChanged);
    HealthComponent->OnDie.AddDynamic(this, &ATurret::OnDie);

    HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
    HPBar->SetupAttachment(BodyMesh);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

    FActorSpawnParameters Params;
    Params.Owner = this;
    Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
    Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

}

void ATurret::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
    if (TurretMeshTemp)
    {
        TurretMesh->SetStaticMesh(TurretMeshTemp);
    }

    UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
    if (BodyMeshTemp)
    {
        BodyMesh->SetStaticMesh(BodyMeshTemp);
    }
}

void ATurret::Destroyed()
{
    Super::Destroyed();

    if (Cannon)
    {
        Cannon->Destroy();
    }
}

void ATurret::Targeting()
{
    if (IsPlayerInRange())
    {
        RotateToPlayer();

        FHitResult HitResult;
        FVector TraceStart = this->GetActorLocation();
        FVector TraceEnd = PlayerPawn->GetActorLocation();
        FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("AI Vission Trace")), true, this);
        TraceParams.bReturnPhysicalMaterial = false;

        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
        {
            DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.1f, 0, 5);
            if (HitResult.Actor != PlayerPawn)
            {
                return;
            }
        }
        else
        {
            DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.1f, 0, 5);
        }

        if (CanFire() && Cannon && Cannon->IsReadyToFire())
        {
            Fire();
        }
    }
    
}

void ATurret::RotateToPlayer()
{
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
    FRotator CurrentRotation = TurretMesh->GetComponentRotation();
    TargetRotation.Pitch = CurrentRotation.Pitch;
    TargetRotation.Roll = CurrentRotation.Roll;
    TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

bool ATurret::IsPlayerInRange()
{
    return FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) <= FMath::Square(TargetingRange);
}

bool ATurret::CanFire()
{
    FVector TargetingDir = TurretMesh->GetForwardVector();
    FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
    DirToPlayer.Normalize();
    float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
    return AimAngle <= Accuracy;
}

void ATurret::Fire()
{
    if (Cannon)
    {
        Cannon->Fire();
    }
}

void ATurret::OnHealthChanged_Implementation(float Damage)
{
    UE_LOG(LogTankogeddon, Log, TEXT("Turret %s taked damage:%f "), *GetName(), Damage);
    if (Cast<UHPBarWidget>(HPBar->GetUserWidgetObject()))
    {
        UHPBarWidget* HP = Cast<UHPBarWidget>(HPBar->GetUserWidgetObject());
        HP->SetHPValue(HealthComponent->GetHealthState());
    }
}

void ATurret::OnDie_Implementation()
{
    Destroy();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (PlayerPawn)
    {
        Targeting();
    }
}

void ATurret::TakeDamage(const FDamageData& DamageData)
{
    HealthComponent->TakeDamage(DamageData);

}

int32 ATurret::GetScores() const
{
    return DestructionScores;
}