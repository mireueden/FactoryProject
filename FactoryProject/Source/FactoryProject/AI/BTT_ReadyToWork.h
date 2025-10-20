// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ReadyToWork.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UBTT_ReadyToWork : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ReadyToWork();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
