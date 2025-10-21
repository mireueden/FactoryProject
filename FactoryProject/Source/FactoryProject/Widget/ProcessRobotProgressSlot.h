// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AI/DeliveryRobot.h"

#include "Components/TextBlock.h"

#include "ProcessRobotProgressSlot.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UProcessRobotProgressSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RobotName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProgressRecipe;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentProgress;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentTargetCell;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentState;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentProcess;


public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateSlot(ADeliveryRobot* Robot);

};
