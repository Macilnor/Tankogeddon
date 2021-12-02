// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    void SetHPValue(float HPValue);
protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    class UProgressBar * HPBar;
};
