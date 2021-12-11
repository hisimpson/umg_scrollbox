// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "TouchScrollItem.h"
#include "TouchScrollWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMGDEMO_API UTouchScrollWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTouchScrollWidget(const FObjectInitializer& ObjectInitializer);
	class UUserWidget* GetSelectItem();
	void CheckItem(int index, ECheckBoxState InCheckedState);
	void CheckItem(UUserWidget* Widget, ECheckBoxState InCheckedState);

protected:
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual bool Initialize() override;
	virtual void SynchronizeProperties() override;

	//아이템이 클릭 될때, UTouchScrollWidget을 상속하고, 여기서 클릭 이벤트를 처리한다.
	virtual void ClickItem(UUserWidget* UserWidget);
	virtual void ScrollFocusItem(UUserWidget* Widget, EScrollStatus ScrollStatus);

	UFUNCTION()
	void OnButtonTest();
	UFUNCTION()
	void OnButtonDown();
	UFUNCTION()
	void OnButtonUp();

	void BindPointer();
	void AddEvent();
	void AddChild(UUserWidget* UserWidget);

	void BindTouchScrollEvent(UUserWidget* UserWidget);
	
	UFUNCTION()
	void OnClickListItem(UWidget* Widget);
	UFUNCTION()
	void OnScrollClick(UWidget* Widget);
	UFUNCTION()
	void OnScrollMove(UWidget* Widget, FVector2D DeltaPos, EScrollStatus ScrollStatus, bool NoneRun);

protected:
	UPROPERTY()
	class UTouchButton* TouchButton_Test;

	TWeakObjectPtr<class UButton> ButtonUpPtr;
	
	TWeakObjectPtr<class UButton> ButtonDownPtr;

	TWeakObjectPtr<class UScrollBox> ScrollBoxPtr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString ButtonDownName;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString ButtonUpName;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString ScrollBoxName;

	TWeakObjectPtr<class UUserWidget> SelectItemPtr;
	TWeakObjectPtr<class UUserWidget> FocusItemPtr;
};
