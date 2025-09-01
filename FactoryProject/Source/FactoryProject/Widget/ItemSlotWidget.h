// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/ItemStorageStruct.h"
#include "DataAsset/ItemBasicDataAsset.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/button.h"
#include "Components/CheckBox.h"
#include "ItemSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotChecked, UItemSlotWidget*, SlotWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUpdate, UItemSlotWidget*, SlotWidget);


/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

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

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* SelectChecker;
	
	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnSlotChecked OnSlotChecked;

	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnSlotUpdate OnSlotUpdate;

	UFUNCTION(BlueprintCallable)
	void HandleCheckBoxChanged(bool bIsChecked);

	//UPROPERTY(meta = (BindWidget))
	//class UButton* SelectCheckBtn;

};
