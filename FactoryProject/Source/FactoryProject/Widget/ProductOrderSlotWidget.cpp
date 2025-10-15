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
		const FItemStorageStruct* FoundStorage = ItemManager->StorageList.FindByPredicate(
			[&](const FItemStorageStruct& StorageElement)
			{
				return StorageElement.ItemData == RecipeElement.ItemData;
			});

		if (!FoundStorage)
		{
			UE_LOG(LogTemp, Warning, TEXT("해당 ItemData를 가진 Storage를 찾을 수 없습니다: %s"),
				*RecipeElement.ItemData->ItemName);
			continue;
		}

		UProductRequiredItemSlot* NewSlot = CreateWidget<UProductRequiredItemSlot>(this, ProductRequiredItemSlotWidgetClass);
		if (NewSlot)
		{
			NewSlot->SetUpRequiredItemSlot(RecipeElement, *FoundStorage);
			RequiredItemContainer->AddChildToVerticalBox(NewSlot);
			RequiredItemSlotWidgetList.Add(NewSlot);
		}
	}
}

void UProductOrderSlotWidget::HandleCheckBoxChanged(bool bIsChecked)
{
	OnProductOrderSlotChecked.Broadcast(this);
}

