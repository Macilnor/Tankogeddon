// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBarWidget.h"

#include "Components/ProgressBar.h"

void UHPBarWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHPBarWidget::SetHPValue(float HPValue)
{
    HPBar->SetPercent(HPValue);
}
