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
	
	// 추후 Anim 진행 상황에 맞춰서 실행하게 위치 바꿔야함
	//ProductCell->CraftingProduct();


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

	int32 FinishedCount = 0;


	for (const ACellRobotArm* Arm : ArmList)
	{
		if (!IsValid(Arm)) continue;

		if (!Arm->bIsProcess) // 처리 끝난 Arm
		{
			FinishedCount++;
		}
	}


	if (FinishedCount == ArmList.Num()) // RobotArm 모두 Animation 완료
	{
		UE_LOG(LogTemp, Warning, TEXT("[AssemblyProcess] All Arms finished process"));

		Robot->TargetCell->FloorMove();
		// FloorMove가 진행이 완료 된 이후, Delegate를 통해 
		// Robot이 스스로 Detach & Delagate해제 & 
		

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}


}
