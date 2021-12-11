// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollItem.h"
#include "Components/TextBlock.h"

#pragma optimize( "", off ) 

UScrollItem::UScrollItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


bool UScrollItem::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	return true;
}


void UScrollItem::SetItem(FString str)
{
	Data_SetText(str);
	if (TextBlock_Text)
		TextBlock_Text->SetText(FText::FromString(str));
}

void UScrollItem::Data_SetText(FString str)
{
	ItemText = str;
}

FString& UScrollItem::Data_GetText()
{
	return ItemText;
}

#pragma optimize( "", on ) 