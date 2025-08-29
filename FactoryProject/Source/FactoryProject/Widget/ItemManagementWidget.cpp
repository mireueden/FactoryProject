// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ItemManagementWidget.h"

void UItemManagementWidget::NativeConstruct()
{
    Super::NativeConstruct();

	if (!ItemSlotWidgetClass || !ItemSlotContainer) return;

	// 기존 슬롯 제거
	ItemSlotContainer->ClearChildren();
	SlotWidgetList.Empty();

    for (const FItemStorageStruct& Item : StorageList)
    {
        UItemSlotWidget* NewSlot = CreateWidget<UItemSlotWidget>(this, ItemSlotWidgetClass);
        if (NewSlot)
        {
            // 슬롯 데이터 초기화
            NewSlot->SetupSlot(Item);

            NewSlot->SetPadding(FMargin(5.0f));
            ItemSlotContainer->AddChildToVerticalBox(NewSlot);

            SlotWidgetList.Add(NewSlot);
        }
    }
    UpdateAddOrderNum();
}

void UItemManagementWidget::UpdateAddOrderNum()
{
    AddOrderNumText->SetText(
        FText::FromString(FString::FromInt(AddOrderNum)));
}
