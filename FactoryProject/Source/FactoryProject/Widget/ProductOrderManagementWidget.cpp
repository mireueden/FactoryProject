// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ProductOrderManagementWidget.h"

void UProductOrderManagementWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnVisibilityChanged.AddDynamic(this, &UProductOrderManagementWidget::HandleVisibilityChanged);

	UpdateProductSlotList();
}

void UProductOrderManagementWidget::UpdateProductSlotList()
{
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

	if (OrderButton) OrderButton->SetIsEnabled(false);
}


void UProductOrderManagementWidget::HandleVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		UpdateProductSlotList();
	}
}

void UProductOrderManagementWidget::Order()
{
	if (SelectedIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Order"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Order"));

	if (!ProductSlotWidgetList.IsValidIndex(SelectedIndex)) return;
	UProductOrderSlotWidget* SelectedSlot = ProductSlotWidgetList[SelectedIndex];
	if (!SelectedSlot) return;

	bool bCanOrder = true;

	for (int32 i = 0; i < SelectedSlot->RequiredItemSlotWidgetList.Num(); ++i)
	{
		UProductRequiredItemSlot* ReqSlot = SelectedSlot->RequiredItemSlotWidgetList[i];
		if (!ReqSlot) continue;


		// RecipeData는 ProductManager->ProductList[SelectedIndex]->RecipeData[i]로 접근
		UProductRecipeDataAsset* ProductData = ProductManager->ProductList[SelectedIndex];
		if (!ProductData || !ProductData->RecipeData.IsValidIndex(i)) continue;

		const FProductRecipeStruct& Recipe = ProductData->RecipeData[i];

		if (!ProductManager->ItemManager)
		{
			UE_LOG(LogTemp, Error, TEXT("ItemManager is nullptr!"));
			return;
		}
		
		// ItemManager에서 해당 아이템 찾기
		FItemStorageStruct* Storage = ProductManager->ItemManager->StorageList.FindByPredicate(
			[&](const FItemStorageStruct& Elem)
			{
				return Elem.ItemData == Recipe.ItemData;
			});

		if (!Storage)
		{
			UE_LOG(LogTemp, Warning, TEXT("No storage found for %s"), *Recipe.ItemData->ItemName);
			bCanOrder = false;
			break;
		}

		// 수량 체크
		if (Storage->CurrentItemCount < Recipe.RequiredItem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough items for %s"), *Recipe.ItemData->ItemName);
			bCanOrder = false;
			break;
		}
	}

	// 주문 실행
	if (bCanOrder)
	{
		UProductRecipeDataAsset* ProductData = ProductManager->ProductList[SelectedIndex];
		if (!ProductData) return;

		for (const FProductRecipeStruct& Recipe : ProductData->RecipeData)
		{
			FItemStorageStruct* Storage = ProductManager->ItemManager->StorageList.FindByPredicate(
				[&](const FItemStorageStruct& Elem) { return Elem.ItemData == Recipe.ItemData; }
			);

			if (Storage)
			{
				Storage->CurrentItemCount -= Recipe.RequiredItem;
			}
		}
		OnProductOrderRequested.Broadcast(ProductData);


		UpdateProductSlotList();
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

		if (SelectedSlot->ProductNameText)
		{
			FText ProductName = SelectedSlot->ProductNameText->GetText();
			if (SelectedProductNameText)
			{
				SelectedProductNameText->SetText(ProductName);
			}
		}
	}

	bIsCheckedSlot = (SelectedIndex != INDEX_NONE);

	UpdateOrderBtn();
}

void UProductOrderManagementWidget::UpdateOrderBtn()
{
	if (OrderButton) OrderButton->SetIsEnabled(bIsCheckedSlot);

	if (!bIsCheckedSlot && SelectedProductNameText)
		SelectedProductNameText->SetText(FText::FromString(TEXT("None")));
}
