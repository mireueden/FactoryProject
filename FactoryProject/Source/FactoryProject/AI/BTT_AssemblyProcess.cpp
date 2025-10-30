// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_AssemblyProcess.h"

#include "Object/ItemProductionCell.h"
#include "DeliveryRobot.h"
#include "DeliveryRobotController.h"

UBTT_AssemblyProcess::UBTT_AssemblyProcess()
{
	NodeName = TEXT("AssemblyProcess");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_AssemblyProcess::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController->GetCharacter());

	if (!Robot || !Robot->TargetCell)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTT_AssemblyProcess::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController->GetCharacter());
	AItemProductionCell* ProductCell = Robot->TargetCell;
	TArray<ACellRobotArm*>& ArmList = ProductCell->CellRobotArmList;

	if (!Robot->TargetCell || !IsValid(ProductCell) || ArmList.Num() == 0)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	int32 FinishedCount = 0;

	for (const ACellRobotArm* Arm : ArmList)
	{
		if (!IsValid(Arm)) continue;

		if (!Arm->bIsProcess)
		{
			FinishedCount++;
		}
	}

	if (FinishedCount == ArmList.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("[AssemblyProcess] All Arms finished process"));

		Robot->TargetCell->FloorMove();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
