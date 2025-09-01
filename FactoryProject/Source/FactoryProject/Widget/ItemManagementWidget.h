// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/ItemStorageStruct.h"
#include "DataAsset/ItemBasicDataAsset.h"
#include "DataAsset/ItemManagerActor.h"
#include "DataAsset/ItemManagerObject.h"
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
	AItemManagerActor* ItemManager;
	//UItemManagerObject* ItemManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItemStorageStruct> StorageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<UItemSlotWidget*> SlotWidgetList;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UItemSlotWidget* SelectedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemStorageStruct SelectedStruct;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 SelectedIndex;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class UItemSlotWidget> ItemSlotWidgetClass;





	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ItemSlotContainer;


public:

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

	UFUNCTION(BlueprintCallable)
	void Order();

	UFUNCTION(BlueprintCallable)
	void UpdateOrderBtn();


	UPROPERTY(BlueprintReadWrite)
	int32 AddOrderNum = 0;

public:
	UFUNCTION(BlueprintCallable)
	void SelectCheckUpdate(UItemSlotWidget* SelectedSlot);

	bool bIsCheckedSlot;


	void CheckItemInFactoy();

};
