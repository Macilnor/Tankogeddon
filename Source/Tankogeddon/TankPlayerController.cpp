// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();

    TankPawn = Cast<ATankPawn>(GetPawn());
    bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
    InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
    InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
    InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATankPlayerController::FireSpecial);
}

void ATankPlayerController::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TankPawn)
        return;
    
    FVector MousePosition, MouseDirection;
    DeprojectMousePositionToWorld(MousePosition, MouseDirection);
    MousePosition.Z = TankPawn->GetActorLocation().Z;
    FVector TurretTargetDirection = MousePosition - TankPawn->GetActorLocation();
    TurretTargetDirection.Normalize();
    const FVector TurretTargetPosition = TankPawn->GetActorLocation() + TurretTargetDirection * 1000.f;
    TankPawn->SetTurretTargetPosition(TurretTargetPosition);
    DrawDebugLine(GetWorld(), TankPawn->GetActorLocation(), TurretTargetPosition, FColor::Green, false, 0.1f,0, 5.f);
}

void ATankPlayerController::Fire()
{
    if (TankPawn)
    {
        TankPawn->Fire();
    }
}

void ATankPlayerController::FireSpecial()
{
    if (TankPawn)
    {
        TankPawn->FireSpecial();
    }
}

void ATankPlayerController::MoveForward(const float InAxisValue)
{
    if (TankPawn)
    {
        TankPawn->MoveForward(InAxisValue);
    }
}

void ATankPlayerController::RotateRight(const float InAxisValue)
{
    if (TankPawn)
    {
        TankPawn->RotateRight(InAxisValue);
    }
}
