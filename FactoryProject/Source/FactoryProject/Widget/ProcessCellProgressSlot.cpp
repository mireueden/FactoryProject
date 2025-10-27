// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ProcessCellProgressSlot.h"

#include "AI/DeliveryRobot.h"


void UProcessCellProgressSlot::NativeConstruct()
{
	Super::NativeConstruct();

}

void UProcessCellProgressSlot::UpdateSlot(AItemProductionCell* Cell)
{
	// GetActorLabel : Editor 전용 함수 (패키징시 실행 X)
	CellName->SetText(
		FText::FromString(Cell->GetActorLabel()));
	
	ProgressItem->SetText(
		FText::FromString(Cell->ProductProcessData->ItemName));

	if (const UEnum* EnumPtr = StaticEnum<ECellProgressState>())
	{
		FText ProcessText = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Cell->CurrentState));
		CurrentState->SetText(ProcessText);
	}

	if (!Cell->ProcessRobot)
	{
		CurrentRobot->SetText(
			FText::FromString(TEXT("None")));
	}
	else
	{
		CurrentRobot->SetText(
			FText::FromString(Cell->ProcessRobot->GetName()));
	}

}

