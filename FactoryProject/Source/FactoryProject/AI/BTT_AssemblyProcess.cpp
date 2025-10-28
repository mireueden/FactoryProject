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


	AItemProductionCell* ProductCell = Robot->TargetCell;
	ProductCell->ReadyToCraftingProduct();
	ProductCell->FloorMove();

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
	if (!Robot->TargetCell)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AItemProductionCell* ProductCell = Robot->TargetCell;
	if (!IsValid(ProductCell))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	TArray<ACellRobotArm*>& ArmList = ProductCell->CellRobotArmList;
	if (ArmList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AssemblyProcess] No Robot Arms in Cell!"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	bool bAllProcessing = true;
	bool bAllFinished = true;

	for (const ACellRobotArm* Arm : ArmList)
	{
		if (!IsValid(Arm)) continue;

		if (Arm->bIsProcess)
			bAllFinished = false; // 진행중인 RobotArm잇음
		else
			bAllProcessing = false; // 진행중이 아닌 RobotArm 있음
	}

	if (bAllProcessing)	// RobotArm 모두 Animation 진행 중
	{
		//UE_LOG(LogTemp, Warning, TEXT("[AssemblyProcess] All Arms are processing"));
	}
	else if (bAllFinished) // RobotArm 모두 Animation 완료
	{
		UE_LOG(LogTemp, Warning, TEXT("[AssemblyProcess] All Arms finished process"));
		ProductCell->FloorMove();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	//else // RobotArm 일부 진행 중
	//	UE_LOG(LogTemp, Warning, TEXT("[AssemblyProcess] other Arm states, waiting..."));


}
