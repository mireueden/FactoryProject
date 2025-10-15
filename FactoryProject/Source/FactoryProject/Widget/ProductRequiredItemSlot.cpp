// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ProductRequiredItemSlot.h"

void UProductRequiredItemSlot::NativeConstruct()
{
	Super::NativeConstruct();


}

void UProductRequiredItemSlot::SetUpRequiredItemSlot(const FProductRecipeStruct& ItemRecipe, const FItemStorageStruct& ItemStorage)
{
	ItemNameText->SetText(
		FText::FromString(ItemRecipe.ItemData->ItemName));

	ItemIndexText->SetText(
		FText::FromString(FString::FromInt(ItemRecipe.ItemData->ItemIndex)));

	// 아래의 Item은 무조건 같은 ItemDataAsset이어야함
	// 현재 보유 중인 Item 개수
	CurrentItemCountText->SetText(
		FText::FromString(FString::FromInt(ItemStorage.CurrentItemCount)));
	// 만드는데 필요한 아이템 개수
	RequiredItemCountText->SetText(
		FText::FromString(FString::FromInt(ItemRecipe.RequiredItem)));
}



