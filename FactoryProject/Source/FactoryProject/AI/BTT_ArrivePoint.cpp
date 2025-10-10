// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ArrivePoint.h"

#include "DeliveryRobot.h"
#include "DeliveryRobotController.h"

UBTT_ArrivePoint::UBTT_ArrivePoint()
{
	NodeName = TEXT("ArrivePoint");
}

EBTNodeResult::Type UBTT_ArrivePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	UE_LOG(LogTemp, Warning, TEXT("Call UBTT_ArrivePoint"));
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		return EBTNodeResult::Failed;
	}

	ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
	ADeliveryRobot* Ch = Cast<ADeliveryRobot>(AIController->GetCharacter());

	uint8 StateValue = BBComp->GetValueAsEnum(RobotStateKey.SelectedKeyName);
	ERobotState CurrentState = static_cast<ERobotState>(StateValue);

	BBComp->SetValueAsEnum(TEXT("RobotState"), static_cast<uint8>(ERobotState::Waiting));

	return EBTNodeResult::Succeeded;
}
