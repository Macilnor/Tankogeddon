// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TankogeddonHUD.generated.h"

UENUM()
enum class EWidgetID : uint8
{
	Wid_None,
	Wid_GameOver,
	Wid_MainMenu,
	Wid_PauseMenu,
	Wid_PlayerStatus,
};

UCLASS()
class TANKOGEDDON_API ATankogeddonHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	UUserWidget * CurrentWidget;

	EWidgetID CurrentWidgetID;

	void RemoveCurrentWidgetFromViewport();
	UUserWidget * AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	UUserWidget * UseWidget(EWidgetID WidgetID, bool RemovePrevious = true,
		int32 ZOrder = 0);

	UUserWidget* GetCurrentWidget();
};
