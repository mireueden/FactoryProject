// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ProcessProgressWidget.h"

void UProcessProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HandleProcessUpdate();

	if (RobotManager)
	{
		// 델리게이트 바인딩
		RobotManager->OnDeliveryRobotUpdate.AddDynamic(this, &UProcessProgressWidget::HandleProcessUpdate);
	}
}

void UProcessProgressWidget::HandleProcessUpdate()
{
	UpdateProgressRobot();
	UpdateProgressCell();
}

void UProcessProgressWidget::UpdateProgressRobot()
{
	UE_LOG(LogTemp, Error, TEXT("Call Func UpdateProgressRobot"));

	if (!RobotManager)
	{
		UE_LOG(LogTemp, Error, TEXT("RobotManager is nullptr in %s"), *GetName());
		return;
	}	

	if (!ProcessRobotProgressSlotWidgetClass)
		return;

	// DeliveryRobots robotlist
	// ProductionCellList celllist

	ProgressRobotsContainer->ClearChildren();

	UE_LOG(LogTemp, Error, TEXT("RobotManager->DeliveryRobots Num %d"), RobotManager->DeliveryRobots.Num());

	for (int32 i = 0; i < RobotManager->DeliveryRobots.Num(); ++i)
	{
		UE_LOG(LogTemp, Error, TEXT("RobotManager->DeliveryRobots %d"), i);
		ADeliveryRobot* Robot = RobotManager->DeliveryRobots[i];

		if (!IsValid(Robot))
		{
			UE_LOG(LogTemp, Warning, TEXT("Robot %d is invalid (nullptr or pending kill)"), i);
			continue; 
		}

		UProcessRobotProgressSlot* NewSlot = CreateWidget<UProcessRobotProgressSlot>(this, ProcessRobotProgressSlotWidgetClass);

		if (NewSlot)
		{
			NewSlot->UpdateSlot(Robot);
			NewSlot->SetPadding(FMargin(5.0f));

			ProgressRobotsContainer->AddChildToVerticalBox(NewSlot);

			ProcessRobotProgressSlotList.Add(NewSlot);
		}
	}

	if (ProcessRobotProgressSlotList.Num() == 0)
		NotFoundRobotText->SetVisibility(ESlateVisibility::Visible);
	else
		NotFoundRobotText->SetVisibility(ESlateVisibility::Hidden);

	UE_LOG(LogTemp, Warning, TEXT("End Func UpdateProgressRobot"));
}

void UProcessProgressWidget::UpdateProgressCell()
{
	UE_LOG(LogTemp, Error, TEXT("Call Func UpdateProgressCell"));

	if (!RobotManager)
	{
		UE_LOG(LogTemp, Error, TEXT("RobotManager is nullptr in %s"), *GetName());
		return;
	}

	if (!ProcessCellProgressSlotWidgetClass)
		return;

	ProgressCellsContainer->ClearChildren();

	const int32 MaxColumns = 5; // 0~4
	const int32 MaxRows = 4;    // 0~3

	for (int32 i = 0; i < RobotManager->ProductionCellList.Num(); ++i)
	{
		AItemProductionCell* Cell = RobotManager->ProductionCellList[i];

		UProcessCellProgressSlot* NewSlot = CreateWidget<UProcessCellProgressSlot>(this, ProcessCellProgressSlotWidgetClass);

		if (NewSlot)
		{
			NewSlot->UpdateSlot(Cell);
			NewSlot->SetPadding(FMargin(5.0f));

			// 계산: i = 0~19까지라면 (Row 0~3, Column 0~4)
			int32 Row = i / MaxColumns;
			int32 Col = i % MaxColumns;

			// Row, Column에 배치
			if (UUniformGridSlot* GridSlot = ProgressCellsContainer->AddChildToUniformGrid(NewSlot, Row, Col))
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
			
			if (!Cell->OnCellStateChanged.IsAlreadyBound(this, &UProcessProgressWidget::HandleProcessUpdate))
			{
				Cell->OnCellStateChanged.AddDynamic(this, &UProcessProgressWidget::HandleProcessUpdate);
			}
		}
	}

	if (RobotManager->ProductionCellList.Num() == 0)
		NotFoundCellText->SetVisibility(ESlateVisibility::Visible);
	else
		NotFoundCellText->SetVisibility(ESlateVisibility::Hidden);

}
