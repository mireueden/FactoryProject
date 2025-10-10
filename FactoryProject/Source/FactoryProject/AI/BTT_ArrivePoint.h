// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTT_ArrivePoint.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UBTT_ArrivePoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ArrivePoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector RobotStateKey;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
