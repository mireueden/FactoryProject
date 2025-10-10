// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_DeliveryItem.h"

UBTT_DeliveryItem::UBTT_DeliveryItem()
{
	NodeName = TEXT("DeliveryItem");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_DeliveryItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UBTT_DeliveryItem::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
