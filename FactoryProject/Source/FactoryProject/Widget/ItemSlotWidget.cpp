// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ItemSlotWidget.h"

void UItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectChecker)
	{
		SelectChecker->OnCheckStateChanged.AddDynamic(this, &UItemSlotWidget::HandleCheckBoxChanged);
	}
}

void UItemSlotWidget::SetupSlot(const FItemStorageStruct& ItemStorage)
{	
	ItemNameText->SetText(
		FText::FromString(ItemStorage.ItemData->ItemName));
	ItemIndexText->SetText(
		FText::FromString(FString::FromInt(ItemStorage.ItemData->ItemIndex)));

	CurrentCountText->SetText(
		FText::FromString(FString::FromInt(ItemStorage.CurrentItemCount)));
	MaxCountText->SetText(
		FText::FromString(FString::FromInt(ItemStorage.MaxStorageCount)));
}

void UItemSlotWidget::HandleCheckBoxChanged(bool bIsChecked)
{
	OnSlotChecked.Broadcast(this);
}

