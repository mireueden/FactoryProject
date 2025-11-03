// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_Storage.h"

#include "DeliveryRobot.h"
#include "DeliveryRobotController.h"
#include "UObject/UnrealType.h"
#include "Kismet/GameplayStatics.h"


EBTNodeResult::Type UBTT_Storage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

        return EBTNodeResult::Failed;
    }

    ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
    ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController->GetCharacter());

    RobotManager = Cast<ADeliveryRobotManager>(
        UGameplayStatics::GetActorOfClass(Robot->GetWorld(), ADeliveryRobotManager::StaticClass())
    );

    TargetCell = nullptr;
    FirstItem = Robot->ProgressOfProductRecipe.RecipeData[0].ItemData;


    for (AItemProductionCell* Cell : RobotManager->ProductionCellList)
    {
        if (!Cell) continue;

        if (Cell->ProductProcessData == FirstItem)
        {
            if (Cell->CurrentState == ECellProgressState::Empty)
            {
                TargetCell = Cell;
                break;
            }
        }
    }

    if (Robot->CurrentState == ERobotState::Storage && 
        Robot->CurrentProcess == ERobotProcess::ProductProcess &&
        Robot ->TargetCell)
    {
        Robot->TargetCell = TargetCell;
        TargetCell->CurrentState = ECellProgressState::Reserved;

        Robot->CurrentState = ERobotState::Moving;
        Robot->SetTargetCell();
        Robot->SetRobotState();

        UE_LOG(LogTemp, Warning, TEXT("Robot %s moving to Cell %s"),
            *Robot->GetName(), *TargetCell->GetName());

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }
    else if (Robot->CurrentState == ERobotState::Storage &&
            Robot->CurrentProcess == ERobotProcess::Delivery)
    {
        Robot->CurrentState = ERobotState::Returning;
        Robot->SetRobotState();

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

        return EBTNodeResult::Succeeded;
    }

    Robot->CurrentState = ERobotState::Waiting;
    Robot->SetRobotState();

    UE_LOG(LogTemp, Warning, TEXT("No available Cell. Robot %s waiting."), *Robot->GetName());
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    return EBTNodeResult::Failed;

}

void UBTT_Storage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

    //ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
    //ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController->GetCharacter());

    //for (AItemProductionCell* Cell : RobotManager->ProductionCellList)
    //{
    //    if (!Cell) continue;

    //    if (Cell->ProductProcessData == FirstItem)
    //    {
    //        if (Cell->CurrentState == ECellProgressState::Empty)
    //        {
    //            TargetCell = Cell;
    //            break;
    //        }
    //    }
    //}

    //if(Robot->TargetCell != nullptr)
    //{
    //    Robot->TargetCell = TargetCell;
    //    Robot->TargetPoint = TargetCell->GetActorLocation();
    //    Robot->TargetPoint.Z = Robot->GetActorLocation().Z;

    //    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    //}

    //// 이동 가능한 Cell이 없으면 대기 상태 유지
    //Robot->CurrentState = ERobotState::Waiting;
    //Robot->SetRobotState();

    //UE_LOG(LogTemp, Warning, TEXT("No available Cell. Robot %s waiting."), *Robot->GetName());
}
