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

	RequiredItemCountText->SetText(
		FText::FromString(FString::FromInt(ItemRecipe.RequiredItem)));

	CurrentItemCountText->SetText(
		FText::FromString(FString::FromInt(ItemStorage.CurrentItemCount)));
}



