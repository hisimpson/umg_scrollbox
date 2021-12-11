// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchScrollWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "TouchToggleButton.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "TouchButton.h"

#pragma optimize( "", off ) 


void GetScreenSizeToWidgetSize(const FGeometry& geometry, const FVector2D& ScreenPos, FVector2D& OutWidgetSize)
{
	const FVector2D& localSize = geometry.GetLocalSize();
	FVector2D screenPosition = geometry.LocalToAbsolute(FVector2D(0, 0)); //TopLeft
	FVector2D  screenSize = geometry.LocalToAbsolute(localSize) - screenPosition; // BotRight-TopLeft = real size
	OutWidgetSize = ScreenPos * localSize / screenSize;
}

UTouchScrollWidget::UTouchScrollWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ButtonUpPtr = nullptr;
	ButtonDownPtr = nullptr;
	ScrollBoxPtr = nullptr;
	SelectItemPtr = nullptr;
	FocusItemPtr = nullptr;
}

bool UTouchScrollWidget::Initialize()
{
	if(Super::Initialize() == false)
		return false;

	BindPointer();
	AddEvent();
	return true;
}

void UTouchScrollWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	const auto World = GetWorld();
	if (nullptr != World && World->IsPlayInEditor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT(" SynchronizeProperties"));
		BindPointer();
		AddEvent();
	}
}

void UTouchScrollWidget::BindPointer()
{
	if (!ButtonDownName.IsEmpty())
	{
		UButton* button = Cast<UButton>(GetWidgetFromName(*ButtonDownName));
		if (button)
			ButtonDownPtr = button;
	}

	if (!ButtonUpName.IsEmpty())
	{
		UButton* button = Cast<UButton>(GetWidgetFromName(*ButtonUpName));
		if (button)
			ButtonUpPtr = button;
	}

	if (!ScrollBoxName.IsEmpty())
	{
		UScrollBox* ScrollBox = Cast<UScrollBox>(GetWidgetFromName(*ScrollBoxName));
		if (ScrollBox)
			ScrollBoxPtr = ScrollBox;
	}
}

void UTouchScrollWidget::AddEvent()
{
	if (ButtonDownPtr.IsValid())
	{
		UButton* ButtonDown = ButtonDownPtr.Get();
		ButtonDown->OnClicked.RemoveDynamic(this, &UTouchScrollWidget::OnButtonDown);
		ButtonDown->OnClicked.AddDynamic(this, &UTouchScrollWidget::OnButtonDown);

		UButton* ButtonUp = ButtonUpPtr.Get();
		ButtonUp->OnClicked.RemoveDynamic(this, &UTouchScrollWidget::OnButtonUp);
		ButtonUp->OnClicked.AddDynamic(this, &UTouchScrollWidget::OnButtonUp);

		TouchButton_Test->OnClicked.RemoveDynamic(this, &UTouchScrollWidget::OnButtonTest);
		TouchButton_Test->OnClicked.AddDynamic(this, &UTouchScrollWidget::OnButtonTest);
	}
}

void UTouchScrollWidget::OnButtonTest()
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked OnButtonTest"));
}

void UTouchScrollWidget::OnButtonDown()
{
	if (!ScrollBoxPtr.IsValid())
		return;

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	float ScrollOffset = ScrollBox->GetScrollOffset();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return;

	UWidget* Child = ScrollBox->GetChildAt(0);
	FVector2D ChildSize = Child->GetDesiredSize();

	const FGeometry& Geometry = ScrollBox->GetTickSpaceGeometry();
	FVector2D ViewSize = Geometry.GetLocalSize();
	float MaxScrollSize = ChildSize.Y * count;

	if (ScrollOffset + ChildSize.Y < MaxScrollSize - ViewSize.Y)
		ScrollBox->SetScrollOffset(ScrollOffset + ChildSize.Y);
	else
		ScrollBox->SetScrollOffset(MaxScrollSize - ViewSize.Y);
	UE_LOG(LogTemp, Warning, TEXT("Clicked Down"));
}

void UTouchScrollWidget::OnButtonUp()
{
	if (!ScrollBoxPtr.IsValid())
		return;
	UScrollBox* ScrollBox = ScrollBoxPtr.Get();

	float ScrollOffset = ScrollBox->GetScrollOffset();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return;

	UWidget* Child = ScrollBox->GetChildAt(0);
	FVector2D ChildSize = Child->GetDesiredSize();

	if (ScrollOffset - ChildSize.Y < 0)
		ScrollBox->SetScrollOffset(0);
	else
		ScrollBox->SetScrollOffset(ScrollOffset - ChildSize.Y);

	UE_LOG(LogTemp, Warning, TEXT("Clicked Up"));
}

void UTouchScrollWidget::CheckItem(UUserWidget* Widget, ECheckBoxState InCheckedState)
{
	TArray<UWidget*> WidgetList;
	Widget->WidgetTree->GetAllWidgets(WidgetList);
	if (WidgetList.Num() == 0)
		return;

	for (int n = 0; n < WidgetList.Num(); ++n)
	{
		UCheckBox* CheckBox = Cast<UCheckBox>(WidgetList[n]);
		if (CheckBox)
		{
			CheckBox->SetCheckedState(InCheckedState);
			break;
		}
	}
}

void UTouchScrollWidget::CheckItem(int index, ECheckBoxState InCheckedState)
{
	if (!ScrollBoxPtr.IsValid())
		return;
	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return;

	UUserWidget* UserWidget = Cast<UUserWidget>(ScrollBox->GetChildAt(0));
	if(UserWidget)
		CheckItem(UserWidget, InCheckedState);
}

void UTouchScrollWidget::BindTouchScrollEvent(UUserWidget* UserWidget)
{
	TArray<UWidget*> WidgetList;
	UserWidget->WidgetTree->GetAllWidgets(WidgetList);
	if (WidgetList.Num() == 0)
		return;

	for (int n = 0; n < WidgetList.Num(); ++n)
	{
		UTouchToggleButton* touchToggleButton = Cast<UTouchToggleButton>(WidgetList[n]);
		if (touchToggleButton)
		{
			touchToggleButton->OnClickTouchButton.AddDynamic(this, &UTouchScrollWidget::OnClickListItem);
			break;
		}
	}
}

UUserWidget* GetUserWidget(UWidget* Widget)
{
	UWidgetTree* WidgetTree = Cast<UWidgetTree>(Widget->GetOuter());
	if (WidgetTree == nullptr)
		return nullptr;

	UUserWidget* UserWidget = Cast<UUserWidget>(WidgetTree->GetOuter());
	return UserWidget;
}

UUserWidget* UTouchScrollWidget::GetSelectItem()
{
	if (!SelectItemPtr.IsValid())
		return nullptr;

	return SelectItemPtr.Get();
}

void UTouchScrollWidget::ClickItem(UUserWidget* Widget)
{
	UUserWidget* OldItem = GetSelectItem();
	if (OldItem)
		CheckItem(OldItem, ECheckBoxState::Unchecked);

	if(Widget)
		CheckItem(Widget, ECheckBoxState::Checked);

	SelectItemPtr = Widget;
}

void UTouchScrollWidget::ScrollFocusItem(UUserWidget* Widget, EScrollStatus ScrollStatus)
{
	UUserWidget* SelectWidget = nullptr;
	UUserWidget* FocusWidget = nullptr;
	if (SelectItemPtr.IsValid())
		SelectWidget = SelectItemPtr.Get();
	if (FocusItemPtr.IsValid())
		FocusWidget = FocusItemPtr.Get();


	if (ScrollStatus == EScrollStatus::Start || ScrollStatus == EScrollStatus::Move)
	{
		if (FocusWidget != nullptr && SelectWidget != FocusWidget)
			CheckItem(FocusWidget, ECheckBoxState::Unchecked);

		FocusItemPtr = Widget;
		if (FocusWidget != nullptr || SelectWidget != FocusWidget)
			CheckItem(Widget, ECheckBoxState::Checked);
	}
	else if (ScrollStatus == EScrollStatus::End)
	{
		if(SelectWidget != FocusWidget)
			CheckItem(FocusWidget, ECheckBoxState::Unchecked);
		if (SelectWidget != Widget)
			CheckItem(Widget, ECheckBoxState::Unchecked);
		FocusItemPtr = nullptr;
	}
}

void UTouchScrollWidget::AddChild(UUserWidget* UserWidget)
{
	if (!ScrollBoxPtr.IsValid())
		return;
	UTouchScrollItem* ScrollItem = Cast<UTouchScrollItem>(UserWidget);
	if (ScrollItem == nullptr)
		return;

	ScrollItem->OnScrollMoveEvent.AddDynamic(this, &UTouchScrollWidget::OnScrollMove);
	ScrollItem->OnScrollClickEvent.AddDynamic(this, &UTouchScrollWidget::OnScrollClick);

	BindTouchScrollEvent(UserWidget);
		
	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	ScrollBox->AddChild(UserWidget);
}

void UTouchScrollWidget::OnClickListItem(UWidget* Widget)
{
	if (!ScrollBoxPtr.IsValid())
		return;
	UScrollBox* ScrollBox = ScrollBoxPtr.Get();

	UWidget* Parent = Widget->GetParent();
	UUserWidget* UserWidget = nullptr;

	TSubclassOf<UUserWidget> NewWidgetClass;

	while (Parent)
	{
		UserWidget = GetUserWidget(Widget);
		if (UserWidget)
			break;;
		Parent = Parent->GetParent();
	}

	if (UserWidget == nullptr)
		return;

	ClickItem(UserWidget);
}

void UTouchScrollWidget::OnScrollClick(UWidget* Widget)
{
	if (!ScrollBoxPtr.IsValid())
		return;
	UScrollBox* ScrollBox = ScrollBoxPtr.Get();

	UWidget* Parent = Widget->GetParent();
	UUserWidget* UserWidget = Cast<UUserWidget>(Widget);

	if (UserWidget == nullptr)
		return;

	ClickItem(UserWidget);
}

void UTouchScrollWidget::OnScrollMove(UWidget* Widget, FVector2D DeltaPos, EScrollStatus ScrollStatus, bool NoneRun)
{
	UUserWidget* UserWidget = Cast<UUserWidget>(Widget);
	if (UserWidget)
	{
		ScrollFocusItem(UserWidget, ScrollStatus);
	}

	if(!NoneRun)
	{
		if (!ScrollBoxPtr.IsValid())
			return;
		UScrollBox* ScrollBox = ScrollBoxPtr.Get();

		int count = ScrollBox->GetChildrenCount();
		if (count == 0)
			return;

		float OldScrollOffset = ScrollBox->GetScrollOffset();

		FVector2D MovePos = FVector2D::ZeroVector;
		GetScreenSizeToWidgetSize(ScrollBox->GetTickSpaceGeometry(), DeltaPos, MovePos);
		FVector2D offset = MovePos;
		ScrollBox->SetScrollOffset(OldScrollOffset + (-offset.Y));
		float ScrollOffset = ScrollBox->GetScrollOffset();

		if (ScrollOffset < 0)
			ScrollBox->SetScrollOffset(0);

		UWidget* Child = ScrollBox->GetChildAt(0);
		FVector2D ChildSize = Child->GetDesiredSize();

		const FGeometry& Geometry = ScrollBox->GetTickSpaceGeometry();
		FVector2D ViewSize = Geometry.GetLocalSize();
		float MaxScrollSize = ChildSize.Y * count;

		if (ScrollOffset > MaxScrollSize - ViewSize.Y)
			ScrollBox->SetScrollOffset(MaxScrollSize - ViewSize.Y);
	}
}

FReply UTouchScrollWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

#pragma optimize( "", on ) 


/*
UTouchScrollWidget ���� ����

CheckBox�� Check�� TouchToggleButton���� ���
CheckBox�� UnCheck�� UTouchScrollWidget���� ���
*/

/*
#1
class UI_TEST_API UTouchScrollWidget : public UUserWidget
    ....
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
    TWeakObjectPtr<class UButton> ButtonUpPtr;

#2
�̰� �ȵ�. �ֳ��ϸ� UE4 Widget �����Ϳ��� ����Ʈ�� �ϳ��� �Ⱥ��δ�.
�Ʒ��� ���� �ؾ� ��ġ�� ��ư ����Ʈ�� ���δ�.

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	class UButton* ButtonUpPtr;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	//class UButton* ButtonUpPtr;

	UPROPERTY()
	class UScrollBox* ScrollBoxPtr;

#3
TWeakObjectPtr ����
TWeakObjectPtr<AActor> MyWeakActor;
MyWeakActor = MyActor;

TWeakObjectPtr::IsValid()

AActor* Actor = MyWeakActor.Get();

https://dawnarc.com/2018/07/ue4-tsharedptr-tweakobjectptr-and-tuniqueptr/


#4 
����� �������� �ֻ����϶��� �ٸ� ��Ʈ���� �����͸� ������ �ִ�.
������ �ֻ����� �ƴҶ��� �������Ʈ �������� �ϸ� �ٸ� ��Ʈ���� �����͸� ������ ����.
������ ��Ʈ���� �ٸ� ��Ʈ���� �����͸� ������ �ִ�...... 
��� ��Ģ�� �ִ°ǰ�...���� UPROPERTY ������ �߸��Ѱǰ�...
������ �ڷḦ ã�� �ʿ䰡 ����


#5
GetUserWidget �Լ����� Outer --> WidgetTree --> UUserWidget ���ϴ� ��� 


#6
������ ũ�⸦ ���ϱ�
FVector2D UWidget::GetDesiredSize()

#7 Ŭ���Ͽ� ����Ÿ ���
void UScrollView::ClickItem(UUserWidget* Widget)

#8 AddOnScreenDebugMessage ��Ʈ ũ�� ����
FVector2D Scale = FVector2D(3.0f, 3.0f);
GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, item->Data_GetText(), true, Scale);

�Ʒ����� �������� ... ĵ�ٽ����� ���ڿ� ����ϴ� ���
https://qiita.com/unknown_ds/items/759edd4c8b93ad1977cc


#9 GetScreenSizeToWidgetSize  ��ũ�� ����� Widget Size�� ����
�Ʒ� ���������� "How to get the screen size"�� ���� �Ұ�
https://dawnarc.com/2018/12/ue4slate-and-native-umgc-notes/    

*/

