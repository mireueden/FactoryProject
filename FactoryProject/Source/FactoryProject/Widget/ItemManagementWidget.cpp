// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ItemManagementWidget.h"

void UItemManagementWidget::NativeConstruct()
{
    Super::NativeConstruct();

	if (!ItemSlotWidgetClass || !ItemSlotContainer) return;

	// 기존 슬롯 제거
	ItemSlotContainer->ClearChildren();
	SlotWidgetList.Empty();

    for (const FItemStorageStruct& Item : ItemManager->StorageList)
    {
        UItemSlotWidget* NewSlot = CreateWidget<UItemSlotWidget>(this, ItemSlotWidgetClass);
        if (NewSlot)
        {
            // 슬롯 데이터 초기화
            NewSlot->SetupSlot(Item);

            NewSlot->SetPadding(FMargin(5.0f));
            ItemSlotContainer->AddChildToVerticalBox(NewSlot);

            NewSlot->OnSlotChecked.AddDynamic(this, &UItemManagementWidget::SelectCheckUpdate);

            SlotWidgetList.Add(NewSlot);
            
        }
    }
    UpdateAddOrderNum();
    UpdateOrderBtn();
}

void UItemManagementWidget::UpdateAddOrderNum()
{
    AddOrderNumText->SetText(
        FText::FromString(FString::FromInt(AddOrderNum)));
}

void UItemManagementWidget::Order()
{
    // 주문 버튼 클릭시


    // 체크된 slot이 있다?
    if (SelectedIndex != INDEX_NONE)
    {
        // 값을 설정해둔 AddOrderNum을 해당 위젯의 변수에 추가하기
        UE_LOG(LogTemp, Warning, TEXT("Order & CurrentItemCount Increase"));

        int32 CurrentCount = ItemManager->StorageList[SelectedIndex].CurrentItemCount;
        int32 MaxCount = ItemManager->StorageList[SelectedIndex].MaxStorageCount;


        // 실제 생성
        ItemManager->OrderSpawn(SelectedIndex, AddOrderNum);

        SlotWidgetList[SelectedIndex]->SetupSlot(ItemManager->StorageList[SelectedIndex]);
    }
    else if (SelectedIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Can't Order"));
        return;

    }

}

void UItemManagementWidget::SelectCheckUpdate(UItemSlotWidget* SelectedSlot)
{
    SelectedIndex = INDEX_NONE;

    // 나머지 슬롯 해제
    for (int32 i = 0; i < SlotWidgetList.Num(); ++i)
    {
        UItemSlotWidget* SlotWidget = SlotWidgetList[i];
        if (SlotWidget && SlotWidget != SelectedSlot && SlotWidget->SelectChecker)
        {
            SlotWidget->SelectChecker->SetIsChecked(false);
        }
    }

    // SelectedSlot이 실제 체크된 상태인지 확인 후 SelectedIndex 갱신
    if (SelectedSlot && SelectedSlot->SelectChecker && SelectedSlot->SelectChecker->IsChecked())
    {
        SelectedIndex = SlotWidgetList.IndexOfByKey(SelectedSlot);
    }

    bIsCheckedSlot = (SelectedIndex != INDEX_NONE);


    UpdateOrderBtn();
}


void UItemManagementWidget::UpdateOrderBtn()
{
    // 모든 SelectChecker가 false일 경우 OrderButton, IncreaseButton, DecreaseButton 비활성화
    if (OrderButton) OrderButton->SetIsEnabled(bIsCheckedSlot);
    if (IncreaseButton) IncreaseButton->SetIsEnabled(bIsCheckedSlot);
    if (DecreaseButton) DecreaseButton->SetIsEnabled(bIsCheckedSlot);
}


void UItemManagementWidget::CheckItemInFactoy()
{
    

}