// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAsset/ItemStorageStruct.h"
#include "DataAsset/ItemBasicDataAsset.h"

#include "DataAsset/ItemManagerActor.h"
#include "Manager/ProductOrderManager.h"
#include "DataAsset/ItemManagerObject.h"
#include "../Widget/ItemSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/button.h"
#include "Components/VerticalBox.h"

#include "Widget/ProductOrderSlotWidget.h"

#include "ProductOrderManagementWidget.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProductOrderRequested, UProductRecipeDataAsset*, ProductRecipe);

UCLASS()
class FACTORYPROJECT_API UProductOrderManagementWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	AProductOrderManager* ProductManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	AItemManagerActor* ItemManager;
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SelectedProductNameText;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ProductSlotContainer;

	UPROPERTY(meta = (BindWidget))
	class UButton* OrderButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	TSubclassOf<class UProductOrderSlotWidget> ProductOrderSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	TSubclassOf<class UProductRequiredItemSlot> ProductRequiredItemSlotWidgetClass;

	UPROPERTY(BlueprintAssignable, Category = "Order")
	FOnProductOrderRequested OnProductOrderRequested;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<UProductOrderSlotWidget*> ProductSlotWidgetList;


public:
	UFUNCTION(BlueprintCallable)
	void UpdateProductSlotList();

	UFUNCTION()
	void HandleVisibilityChanged (ESlateVisibility InVisibility);

	UFUNCTION(BlueprintCallable)
	void Order();

	UFUNCTION(BlueprintCallable)
	void SelectCheckUpdate(UProductOrderSlotWidget* SelectedSlot);

	UFUNCTION(BlueprintCallable)
	void UpdateOrderBtn();


	bool bIsCheckedSlot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UProductOrderSlotWidget* SelectedWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 SelectedIndex;
};
