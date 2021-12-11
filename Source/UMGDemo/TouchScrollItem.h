// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TouchScrollItem.generated.h"

UENUM(BlueprintType)
enum class EScrollStatus : uint8
{
	None UMETA(DisplayName = "None"),
	Start UMETA(DisplayName = "Start"),
	Move UMETA(DisplayName = "Move"),
	End UMETA(DisplayName = "End")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FScrollMoveEvent, UWidget*, Widget, FVector2D, DeltaPos, EScrollStatus, ScrollStatus, bool, NoneRun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScrollClickEvent, UWidget*, Widget);

/**
 * 
 */
UCLASS()
class UMGDEMO_API UTouchScrollItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UTouchScrollItem(const FObjectInitializer& ObjectInitializer);
	FScrollMoveEvent OnScrollMoveEvent;
	FScrollClickEvent OnScrollClickEvent;

protected:
	virtual bool Initialize() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	FVector2D Delta;
};
