// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TouchScrollItem.h"
#include "ScrollItem.generated.h"


/**
 * 
 */
UCLASS()
class UMGDEMO_API UScrollItem : public UTouchScrollItem
{
	GENERATED_BODY()

public:
	UScrollItem(const FObjectInitializer& ObjectInitializer);
	void SetItem(FString str);

protected:
	virtual bool Initialize() override;

protected:
	UPROPERTY()
	class UTextBlock* TextBlock_Text;

	UPROPERTY()
	FString ItemText;

public:
	void Data_SetText(FString str);
	FString& Data_GetText();
};