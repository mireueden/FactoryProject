// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_GetItem.h"

#include "DeliveryRobot.h"
#include "DeliveryRobotController.h"
#include "UObject/UnrealType.h"

UBTT_GetItem::UBTT_GetItem()
{
	NodeName = TEXT("GetItem");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_GetItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTT_GetItem::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }
    ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
    ADeliveryRobot* Ch = Cast<ADeliveryRobot>(AIController->GetCharacter());

    uint8 StateValue = BBComp->GetValueAsEnum(TEXT("RobotState"));
    ERobotState CurrentState = static_cast<ERobotState>(StateValue);

    if (CurrentState == ERobotState::Arrived)
    {
        UE_LOG(LogTemp, Warning, TEXT("RobotState is Waiting"));
        // 로봇에 적재하는 기능 실행
        Ch->AttachItem();

        BBComp->SetValueAsEnum(TEXT("RobotState"), static_cast<uint8>(ERobotState::Delivery));
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        UEnum* EnumPtr = StaticEnum<ERobotState>();
        if (EnumPtr)
        {
            FString StateName = EnumPtr->GetNameStringByValue(static_cast<int64>(CurrentState));
            UE_LOG(LogTemp, Warning, TEXT("CurrentState is %s"), *StateName);
        }
        else
            UE_LOG(LogTemp, Warning, TEXT("Can't Find EnumPtr"));

    }
}
