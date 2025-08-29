// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/ItemStorageStruct.h"
#include "DataAsset/ItemBasicDataAsset.h"
#include "../Widget/ItemSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/button.h"
#include "Components/VerticalBox.h"
#include "ItemManagementWidget.generated.h"

/**
 * 
 */


UCLASS()
class FACTORYPROJECT_API UItemManagementWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItemStorageStruct> StorageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<UItemSlotWidget*> SlotWidgetList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class UItemSlotWidget> ItemSlotWidgetClass;



	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ItemSlotContainer;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AddOrderNumText;







	UPROPERTY(meta = (BindWidget))
	class UButton* OrderButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* IncreaseButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DecreaseButton;


	UFUNCTION(BlueprintCallable)
	void UpdateAddOrderNum();


	UPROPERTY(BlueprintReadWrite)
	int32 AddOrderNum = 0;
};
