// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/button.h"
#include "Components/CheckBox.h"
#include "DataAsset/ProductRecipeStruct.h"
#include "DataAsset/ItemStorageStruct.h"
#include "ProductRequiredItemSlot.generated.h"


UCLASS()
class FACTORYPROJECT_API UProductRequiredItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetUpRequiredItemSlot(const FProductRecipeStruct& ItemRecipe,const FItemStorageStruct& ItemStorage);
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemIndexText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentItemCountText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RequiredItemCountText;
};
