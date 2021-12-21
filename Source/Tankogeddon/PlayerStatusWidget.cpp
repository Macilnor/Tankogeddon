// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusWidget.h"

#include <string>

#include "Cannon.h"
#include "HealthComponent.h"
#include "MiniMap.h"
#include "TankPawn.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UPlayerStatusWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPlayerStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (Cast<ATankPawn>(GetOwningPlayerPawn())->GetCannon())
    {
        float Ammo = Cast<ATankPawn>(GetOwningPlayerPawn())->GetCannon()->GetAmmo();
        float AmmoState = Cast<ATankPawn>(GetOwningPlayerPawn())->GetCannon()->GetAmmoState();
        AmmoBar->SetPercent(AmmoState);
        AmmoCount->SetText(UKismetTextLibrary::Conv_FloatToText(Ammo,ERoundingMode::HalfFromZero));
    }

    if (Cast<ATankPawn>(GetOwningPlayerPawn()))
    {
        float HPState = Cast<ATankPawn>(GetOwningPlayerPawn())->GetHealthComponent()->GetHealthState();
        float HP = Cast<ATankPawn>(GetOwningPlayerPawn())->GetHealthComponent()->GetHealth();
        float Score = Cast<ATankPawn>(GetOwningPlayerPawn())->GetScores();
        HPBar->SetPercent(HPState);
        HPCount->SetText(UKismetTextLibrary::Conv_FloatToText(HP,ERoundingMode::HalfFromZero));
        PlayerScore->SetText(UKismetTextLibrary::Conv_FloatToText(Score,ERoundingMode::HalfFromZero));
        PlayerMiniMap->SetPlayerPercentPos(Cast<ATankPawn>(GetOwningPlayerPawn())->GetActorLocation(), FVector2D{10000.f, 10000.f});
    }
}
