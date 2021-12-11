// Fill out your copyright notice in the Description page of Project Settings.


#include "MyScrollWidget.h"
#include "ScrollItem.h"

#pragma optimize( "", off )

UMyScrollWidget::UMyScrollWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UMyScrollWidget::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	for (int n = 0; n < 20; ++n)
	{
		FString Name = FString::Printf(TEXT("%04d"), n);
		AddItem(Name);
	}

	return true;
}

void UMyScrollWidget::AddItem(FString str)
{
	FStringClassReference SBWidgetRef(TEXT("/Game/UI/BP_ListItem.BP_ListItem_C"));
	if (UClass* WidgetClass = SBWidgetRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		UScrollItem* ListItem = Cast<UScrollItem>(UserWidget);
		if (ListItem)
			ListItem->SetItem(str);

		UTouchScrollWidget::AddChild(UserWidget);
	}
}


void UMyScrollWidget::ClickItem(UUserWidget* Widget)
{
	Super::ClickItem(Widget);

	if (SelectItemPtr.IsValid())
	{
		UScrollItem* item = Cast< UScrollItem>(SelectItemPtr.Get());
		if (item)
		{
			FVector2D Scale = FVector2D(3.0f, 3.0f);
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, item->Data_GetText(), true, Scale);
		}
	}
}

#pragma optimize( "", on )