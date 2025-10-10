// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DeliveryRobotController.h"

ADeliveryRobotController::ADeliveryRobotController()
{
}

void ADeliveryRobotController::BeginPlay()
{
    Super::BeginPlay();

}

void ADeliveryRobotController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ADeliveryRobotController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    //if (!BlackboardAsset || !BehaviorTree)
    //    return;

    //if (UseBlackboard(BlackboardAsset, BlackboardComp))
    //{
    //    if (RunBehaviorTree(BehaviorTree))
    //    {
    //        UE_LOG(LogTemp, Log, TEXT("Behavior Tree started successfully"));
    //    }
    //    else
    //        UE_LOG(LogTemp, Error, TEXT("RunBehaviorTree() failed."));
    //}
}
