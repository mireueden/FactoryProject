// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Manager/DeliveryRobotManager.h"

#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

#include "Widget/ProcessRobotProgressSlot.h"
#include "Widget/ProcessCellProgressSlot.h"


#include "ProcessProgressWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UProcessProgressWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcessProgress")
	TSubclassOf<class UProcessCellProgressSlot> ProcessCellProgressSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcessProgress")
	TSubclassOf<class UProcessRobotProgressSlot> ProcessRobotProgressSlotWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NotFoundCellText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NotFoundRobotText;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ProgressCellsContainer;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ProgressRobotsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcessProgress")
	TArray<UProcessRobotProgressSlot*> ProcessRobotProgressSlotList;

public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Robots")
	ADeliveryRobotManager* RobotManager;
	// DeliveryRobots robotlist
	// ProductionCellList celllist

	UFUNCTION(BlueprintCallable)
	void UpdateProgressRobot();

	UFUNCTION(BlueprintCallable)
	void UpdateProgressCell();

	UFUNCTION(BlueprintCallable)
	void HandleProcessUpdate();
};
