// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ProductOrderManagementWidget.h"

void UProductOrderManagementWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ProductManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ProductManager is nullptr in %s"), *GetName());
		return;
	}

	if (!ProductOrderSlotWidgetClass || !ProductRequiredItemSlotWidgetClass) return;

	ProductSlotContainer->ClearChildren();
	ProductSlotWidgetList.Empty();

	for (int32 i = 0; i < ProductManager->ProductList.Num(); ++i)
	{
		UProductRecipeDataAsset* Product = ProductManager->ProductList[i];
		if (!Product) continue;

		UProductOrderSlotWidget* NewSlot = CreateWidget< UProductOrderSlotWidget>(this, ProductOrderSlotWidgetClass);

		if (NewSlot)
		{
			NewSlot->SettingProductSlot(ProductManager, ProductManager->ItemManager, i);
			NewSlot->SetUpProductSlot(Product);
			NewSlot->SetPadding(FMargin(5.0f));

			NewSlot->OnProductOrderSlotChecked.AddDynamic(this, &UProductOrderManagementWidget::SelectCheckUpdate);
			
			ProductSlotContainer->AddChildToVerticalBox(NewSlot);

			ProductSlotWidgetList.Add(NewSlot);
		}
	}
}

void UProductOrderManagementWidget::Order()
{
	if (SelectedIndex != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Order"));


		int32 CurrentCount = ItemManager->StorageList[SelectedIndex].CurrentItemCount;
		int32 RequiredItemCount = ProductManager->ProductList[SelectedIndex]->RecipeData[0].RequiredItem;

		// 실제 생성
		//ProductManager->OrderSpawn(SelectedIndex, AddOrderNum);

		ProductSlotWidgetList[SelectedIndex]->SetUpProductSlot(ProductManager->ProductList[SelectedIndex]);
	}
	else if (SelectedIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Order"));
		return;
	}
}

void UProductOrderManagementWidget::SelectCheckUpdate(UProductOrderSlotWidget* SelectedSlot)
{
	SelectedIndex = INDEX_NONE;

	// 나머지 슬롯 해제
	for (int32 i = 0; i < ProductSlotWidgetList.Num(); ++i)
	{
		UProductOrderSlotWidget* SlotWidget = ProductSlotWidgetList[i];
		if (SlotWidget && SlotWidget != SelectedSlot && SlotWidget->SelectChecker)
		{
			SlotWidget->SelectChecker->SetIsChecked(false);
		}
	}

	// SelectedSlot이 실제 체크된 상태인지 확인 후 SelectedIndex 갱신
	if (SelectedSlot && SelectedSlot->SelectChecker && SelectedSlot->SelectChecker->IsChecked())
	{
		SelectedIndex = ProductSlotWidgetList.IndexOfByKey(SelectedSlot);
	}

	bIsCheckedSlot = (SelectedIndex != INDEX_NONE);

	UpdateOrderBtn();
}

void UProductOrderManagementWidget::UpdateOrderBtn()
{
	if (OrderButton) OrderButton->SetIsEnabled(bIsCheckedSlot);
}
