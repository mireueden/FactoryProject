// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "Widget/ProductRequiredItemSlot.h"
#include "DataAsset/ProductRecipeDataAsset.h"


#include "ProductOrderSlotWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProductOrderSlotChecked, UProductOrderSlotWidget*, ProductOrderSlotWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProductOrderSlotUpdate, UProductOrderSlotWidget*, ProductOrderSlotWidget);


UCLASS()
class FACTORYPROJECT_API UProductOrderSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetUpProductSlot(UProductRecipeDataAsset* Product);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SettingProductSlot(AProductOrderManager* ProductManager, AItemManagerActor* ItemManager, int32 ProductIndex);

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProductNameText;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* SelectChecker;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* RequiredItemContainer;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	TSubclassOf<class UProductRequiredItemSlot> ProductRequiredItemSlotWidgetClass;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<UProductRequiredItemSlot*> RequiredItemSlotWidgetList;

	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnProductOrderSlotChecked OnProductOrderSlotChecked;

	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnProductOrderSlotUpdate OnProductOrderSlotUpdate;

	UFUNCTION(BlueprintCallable)
	void HandleCheckBoxChanged(bool bIsChecked);

};
