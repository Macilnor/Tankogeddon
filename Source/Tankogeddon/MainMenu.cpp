// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    if (NewGameBtn)
    {
        NewGameBtn->OnPressed.AddDynamic(this, &ThisClass::OnNewGameClicked);
    }
    if (OptionsBtn)
    {
        OptionsBtn->OnPressed.AddDynamic(this, &ThisClass::OnOptionsClicked);
    }
    if (BackBtn)
    {
        BackBtn->OnPressed.AddDynamic(this, &ThisClass::OnBackClicked);
    }
    
}

void UMainMenu::OnNewGameClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
}

void UMainMenu::OnOptionsClicked()
{
    MainMenuWSwitcher->SetActiveWidgetIndex(1);
}

void UMainMenu::OnBackClicked()
{
    MainMenuWSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenu::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(),GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,false);
}
