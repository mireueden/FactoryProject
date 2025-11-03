// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ArriveStorage.h"

#include "DeliveryRobot.h"
#include "DeliveryRobotController.h"

EBTNodeResult::Type UBTT_ArriveStorage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
    ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController ? AIController->GetPawn() : nullptr);
    if (!Robot) return EBTNodeResult::Failed;

    if (USkeletalMeshComponent* Mesh = Robot->GetMesh())
    {
        Mesh->SetVisibility(true);
        Mesh->SetHiddenInGame(false);
        Mesh->SetComponentTickEnabled(true);
    }

    UE_LOG(LogTemp, Warning, TEXT("Robot mesh restored after MoveTo."));

    return EBTNodeResult::Succeeded;
}
