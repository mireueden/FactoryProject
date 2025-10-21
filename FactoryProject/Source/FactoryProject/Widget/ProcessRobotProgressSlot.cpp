// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ProcessRobotProgressSlot.h"

#include "DataAsset/ProductRecipeDataAsset.h"

void UProcessRobotProgressSlot::NativeConstruct()
{
	Super::NativeConstruct();

}

void UProcessRobotProgressSlot::UpdateSlot(ADeliveryRobot* Robot)
{
	RobotName->SetText(
		FText::FromString(Robot->GetName()));


	if (Robot->ProductRecipeData)
	{
		ProgressRecipe->SetText(
			FText::FromString(Robot->ProductRecipeData->ProductName));



		int32 TotalCount = Robot->ProgressOfProductRecipe.RecipeData.Num();
		int32 CompletedCount = 0;

		for (const FProgressOfItem& Item : Robot->ProgressOfProductRecipe.RecipeData)
		{
			if (Item.ProgressValue == 1)
			{
				CompletedCount++;
			}
		}

		float Percent = 0.f;
		if (TotalCount > 0)
		{
			Percent = (static_cast<float>(CompletedCount) / static_cast<float>(TotalCount)) * 100.f;
		}
		FString PercentString = FString::Printf(TEXT("%.0f%%"), Percent);

		CurrentProgress->SetText(FText::FromString(PercentString));
	}
	else
	{
		ProgressRecipe->SetText(
			FText::FromString(TEXT("No Recipe")));

		CurrentProgress->SetText(FText::FromString(TEXT("None")));
	}





	if (CurrentTargetCell)
	{
		if (IsValid(Robot->TargetCell))
		{
			CurrentTargetCell->SetText(FText::FromString(Robot->TargetCell->GetName()));
		}
		else
		{
			if (Robot->CurrentState == ERobotState::Returning)
			{
				CurrentTargetCell->SetText(FText::FromString(TEXT("Warehouse")));
			}
			else
			{
				CurrentTargetCell->SetText(FText::FromString(TEXT("None")));
			}
		}
	}
	else
	{
		CurrentTargetCell->SetText(
			FText::FromString(TEXT("None")));
	}



	if (const UEnum* EnumPtr = StaticEnum<ERobotState>())
	{
		FText StateText = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Robot->CurrentState));
		CurrentState->SetText(StateText);
	}

	if (const UEnum* EnumPtr = StaticEnum<ERobotProcess>())
	{
		FText ProcessText = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Robot->CurrentProcess));
		CurrentProcess->SetText(ProcessText);
	}

	//CurrentState->SetText(
	//	FText::FromString(FString::FromEnum(Robot->CurrentState)));
	//CurrentProcess->SetText(
	//	FText::FromString(FString::FromEnum(Robot->CurrentProcess)));
}
