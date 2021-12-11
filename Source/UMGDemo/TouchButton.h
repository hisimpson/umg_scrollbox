// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TouchButton.generated.h"

/**
 * 
 */
UCLASS()
class UMGDEMO_API UTouchButton : public UButton
{
	GENERATED_BODY()

protected:
	FReply CustomSlateHandleClicked();
	void CustomSlateHandlePressed();
	void CustomSlateHandleReleased();

	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:
	double LastClickTimer = 0.0f;
};
