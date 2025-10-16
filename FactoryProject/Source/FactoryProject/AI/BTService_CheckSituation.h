// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckSituation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FACTORYPROJECT_API UBTService_CheckSituation : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckSituation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    /** 로봇이 현재 진행할 Recipe 인덱스 */
    UPROPERTY(EditAnywhere, Category = "Check")
    int32 CurrentRecipeIndex = 0;

    /** Blackboard Key에 셀 Actor를 지정 */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector TargetCellKey;
};
