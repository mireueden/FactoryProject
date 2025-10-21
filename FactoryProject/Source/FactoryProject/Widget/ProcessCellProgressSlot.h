// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Object/ItemProductionCell.h"

#include "ProcessCellProgressSlot.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UProcessCellProgressSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CellName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProgressItem;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentState;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentRobot;

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateSlot(AItemProductionCell* Cell);
};
