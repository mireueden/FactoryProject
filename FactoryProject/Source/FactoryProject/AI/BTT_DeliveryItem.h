// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTT_DeliveryItem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FACTORYPROJECT_API UBTT_DeliveryItem : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_DeliveryItem();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
