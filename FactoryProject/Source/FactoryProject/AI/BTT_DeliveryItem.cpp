// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_DeliveryItem.h"

#include "DeliveryRobot.h"
#include "DeliveryRobotController.h"
UBTT_DeliveryItem::UBTT_DeliveryItem()
{
	NodeName = TEXT("DeliveryItem");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_DeliveryItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Call UBTT_DeliveryItemPoint"));
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		return EBTNodeResult::Failed;
	}

	ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController->GetCharacter());

	Robot->CurrentState = ERobotState::Storage;
	Robot->SetRobotState();
	//BBComp->SetValueAsVector(TEXT("ReturnPoint"), Robot->ReturnPoint);

	return EBTNodeResult::Succeeded;
}

void UBTT_DeliveryItem::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
