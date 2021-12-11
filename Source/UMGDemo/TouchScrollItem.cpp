// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchScrollItem.h"

const float CLICK_DELTA = 10.0f;
bool IsTouchPad = false;

bool UseTouchPad()
{
	return IsTouchPad;
}

void SetTouchPad(bool TouchPad)
{
	IsTouchPad = TouchPad;
}

UTouchScrollItem::UTouchScrollItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Delta = FVector2D::ZeroVector;
}

bool UTouchScrollItem::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	return true;
}

FReply UTouchScrollItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UseTouchPad())
		return FReply::Handled();

	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		OnScrollMoveEvent.Broadcast(this, InMouseEvent.GetCursorDelta(), EScrollStatus::Start, false);
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UTouchScrollItem::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UseTouchPad())
		return FReply::Handled();

	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		Delta += FVector2D(FMath::Abs(InMouseEvent.GetCursorDelta().X), FMath::Abs(InMouseEvent.GetCursorDelta().Y));
		OnScrollMoveEvent.Broadcast(this, InMouseEvent.GetCursorDelta(), EScrollStatus::Move, false);
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UTouchScrollItem::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UseTouchPad())
		return FReply::Handled();

	if (!InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		if (Delta.X < CLICK_DELTA && Delta.Y < CLICK_DELTA)
		{
			if (OnScrollClickEvent.IsBound())
				OnScrollClickEvent.Broadcast(this);
		}
		else
		{
			OnScrollMoveEvent.Broadcast(this, InMouseEvent.GetCursorDelta(), EScrollStatus::End, false);
		}
		Delta = FVector2D::ZeroVector;
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UTouchScrollItem::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (InGestureEvent.GetTouchpadIndex() != 0)
		return FReply::Handled();

	OnScrollMoveEvent.Broadcast(this, InGestureEvent.GetCursorDelta(), EScrollStatus::Start, false);
	return FReply::Handled();
}

FReply UTouchScrollItem::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (InGestureEvent.GetTouchpadIndex() != 0)
		return FReply::Handled();

	Delta += FVector2D(FMath::Abs(InGestureEvent.GetCursorDelta().X), FMath::Abs(InGestureEvent.GetCursorDelta().Y));
	if (OnScrollMoveEvent.IsBound())
	{
		OnScrollMoveEvent.Broadcast(this, InGestureEvent.GetCursorDelta(), EScrollStatus::Move, false);
	}

	return FReply::Handled();
}

FReply UTouchScrollItem::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (InGestureEvent.GetTouchpadIndex() != 0)
		return FReply::Handled();
	
	if (Delta.X < CLICK_DELTA && Delta.Y < CLICK_DELTA)
	{
		if (OnScrollClickEvent.IsBound())
			OnScrollClickEvent.Broadcast(this);
	}
	else
	{
		OnScrollMoveEvent.Broadcast(this, InGestureEvent.GetCursorDelta(), EScrollStatus::End, false);
	}

	Delta = FVector2D::ZeroVector;
	return FReply::Handled();
}

