// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "Object/ItemProductionCell.h"
#include "Manager/DeliveryRobotManager.h"

#include "BTT_Storage.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UBTT_Storage : public UBTTaskNode
{
	GENERATED_BODY()


public:
	ADeliveryRobotManager* RobotManager;

	AItemProductionCell* TargetCell;

	UItemBasicDataAsset* FirstItem;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
