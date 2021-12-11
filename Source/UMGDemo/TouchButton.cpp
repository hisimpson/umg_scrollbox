// Fill out your copyright notice in the Description page of Project Settings.

#include "TouchButton.h"
#include "Widgets/Input/SButton.h"
#include "Components/ButtonSlot.h"
#include "Windows/WindowsPlatformTime.h"

#pragma optimize( "", off )

class UMGDEMO_API SCumstomButton : public SButton
{
};


FReply UTouchButton::CustomSlateHandleClicked()
{
	double now = FPlatformTime::Seconds();
	if (now - LastClickTimer < 0.05f)
		return FReply::Unhandled();
	LastClickTimer = now;

	OnClicked.Broadcast();
	return FReply::Handled();
}

void UTouchButton::CustomSlateHandlePressed()
{
	double now = FPlatformTime::Seconds();
	if (now - LastClickTimer < 0.05f)
		return;

	OnPressed.Broadcast();
}

void UTouchButton::CustomSlateHandleReleased()
{
	double now = FPlatformTime::Seconds();
	if (now - LastClickTimer < 0.05f)
		return;

	OnReleased.Broadcast();
}


TSharedRef<SWidget> UTouchButton::RebuildWidget()
{
	MyButton = SNew(SCumstomButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, CustomSlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, CustomSlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, CustomSlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	return MyButton.ToSharedRef();
}

#pragma optimize( "", on )

/*
UMG에서 사용자 정의 불규칙 모양 버튼의 C++ 구현
https://blog.csdn.net/xi_niuniu/article/details/54427370?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~default-2.no_search_link&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~default-2.no_search_link
https://www.twblogs.net/a/5b8e21cc2b71771883431183
https://www.codeleading.com/article/10454668485/
https://www.cxyzjd.com/article/xi_niuniu/54427370
*/