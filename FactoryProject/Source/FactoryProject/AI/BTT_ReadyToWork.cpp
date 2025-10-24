// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ReadyToWork.h"

#include "DeliveryRobot.h"
#include "DeliveryRobotController.h"

UBTT_ReadyToWork::UBTT_ReadyToWork()
{
	NodeName = TEXT("ReadyToWork");
}

EBTNodeResult::Type UBTT_ReadyToWork::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	UE_LOG(LogTemp, Warning, TEXT("Call UBTT_ArrivePoint"));

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
		return EBTNodeResult::Failed;


	ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());

	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController->GetCharacter());


	Robot->TargetCell->CellStateChanged(ECellProgressState::InProgress);
	Robot->CurrentState = ERobotState::Working;
	Robot->SetRobotState();

	Robot->TargetCell->CraftingProduct();

	UE_LOG(LogTemp, Warning, TEXT("Robot %s 도착 → State: Arrived"), *Robot->GetName());

	return EBTNodeResult::Succeeded;
}
