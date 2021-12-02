// Fill out your copyright notice in the Description page of Project Settings.


#include "TankogeddonHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void ATankogeddonHUD::RemoveCurrentWidgetFromViewport()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;

        CurrentWidgetID = EWidgetID::Wid_None;
    }
}

UUserWidget* ATankogeddonHUD::AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
    CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport(ZOrder);
    }

    return CurrentWidget;
}

void ATankogeddonHUD::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        APlayerController * PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(
                PC, nullptr, EMouseLockMode::DoNotLock, false);
            PC->bShowMouseCursor = true;
        }
    }
    if (GetWorld()->GetFirstPlayerController()->GetPawn())
    {
        UseWidget(EWidgetID::Wid_PlayerStatus);
    }
    else
    {
        UseWidget(EWidgetID::Wid_MainMenu);
    }

}

UUserWidget* ATankogeddonHUD::UseWidget(EWidgetID WidgetID, bool RemovePrevious, int32 ZOrder)
{
    if (CurrentWidgetID == WidgetID) return CurrentWidget;

    if (RemovePrevious)
    {
        RemoveCurrentWidgetFromViewport();
    }

    TSubclassOf<UUserWidget> WidgetClassToUse =
       WidgetClasses.FindRef(WidgetID);
    if (WidgetClassToUse.Get())
    {
        CurrentWidgetID = WidgetID;
        return AddWidgetToViewportByClass(WidgetClassToUse, ZOrder);
    };

    return nullptr;

}

UUserWidget* ATankogeddonHUD::GetCurrentWidget()
{
    return CurrentWidget;
}
