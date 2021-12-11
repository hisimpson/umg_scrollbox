// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Main.generated.h"

/**
 * 
 */
UCLASS()
class UMGDEMO_API UUI_Main : public UUserWidget
{
	GENERATED_BODY()

public:
	UUI_Main(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;
};
