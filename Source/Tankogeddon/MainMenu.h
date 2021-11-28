// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton * NewGameBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton * OptionsBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton * BackBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton * QuitBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UWidgetSwitcher * MainMenuWSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LoadLevelName;

	UFUNCTION()
	void OnNewGameClicked();
	UFUNCTION()
	void OnOptionsClicked();
	UFUNCTION()
	void OnBackClicked();
	UFUNCTION()
	void OnQuitClicked();
};
