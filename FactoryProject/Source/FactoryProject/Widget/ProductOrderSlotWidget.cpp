// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ProductOrderSlotWidget.h"

#include "DataAsset/ItemManagerActor.h"
#include "Manager/ProductOrderManager.h"


void UProductOrderSlotWidget::NativeConstruct()
{
	if (SelectChecker)
	{
		SelectChecker->OnCheckStateChanged.AddDynamic(this, &UProductOrderSlotWidget::HandleCheckBoxChanged);
	}
}

void UProductOrderSlotWidget::SetUpProductSlot(UProductRecipeDataAsset* Product)
{
	ProductNameText->SetText(
		FText::FromString(Product->ProductName));
}

void UProductOrderSlotWidget::SettingProductSlot(AProductOrderManager* ProductManager, AItemManagerActor* ItemManager, int32 ProductIndex)
{
	if (!ProductManager || !ItemManager || !ProductRequiredItemSlotWidgetClass) return;
	if (!ProductManager->ProductList.IsValidIndex(ProductIndex)) return;

	UProductRecipeDataAsset* ProductData = ProductManager->ProductList[ProductIndex];
	if (!ProductData) return;

	RequiredItemContainer->ClearChildren();
	RequiredItemSlotWidgetList.Empty();

	for (const FProductRecipeStruct& RecipeElement : ProductData->RecipeData)
	{
		UProductRequiredItemSlot* NewSlot = CreateWidget<UProductRequiredItemSlot>(this, ProductRequiredItemSlotWidgetClass);
		if (NewSlot)
		{
			NewSlot->SetUpRequiredItemSlot(RecipeElement, ItemManager->StorageList[ProductIndex]);  // 여기서 자꾸 튕김
			NewSlot->SetPadding(FMargin(30.0f));
			RequiredItemContainer->AddChildToVerticalBox(NewSlot);
			RequiredItemSlotWidgetList.Add(NewSlot);
		}
	}

}

void UProductOrderSlotWidget::HandleCheckBoxChanged(bool bIsChecked)
{
	OnProductOrderSlotChecked.Broadcast(this);

}

