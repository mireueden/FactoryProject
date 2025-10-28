// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTT_AssemblyProcess.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UBTT_AssemblyProcess : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_AssemblyProcess();


protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
