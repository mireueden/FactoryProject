// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/ItemStorageStruct.h"
#include "DataAsset/ItemBasicDataAsset.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/button.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 슬롯에 데이터 바인딩하는 함수
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetupSlot(const FItemStorageStruct& ItemStorage);

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemIndexText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentCountText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxCountText;

};
