// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckSituation.h"

#include "AI/DeliveryRobot.h"
#include "DeliveryRobotController.h"
#include "Manager/DeliveryRobotManager.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckSituation::UBTService_CheckSituation()
{
    bCreateNodeInstance = true;
    bNotifyTick = true;
    NodeName = TEXT("Check Situation");
    Interval = 1.0f;
}

void UBTService_CheckSituation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    ADeliveryRobotManager* Manager = Cast<ADeliveryRobotManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADeliveryRobotManager::StaticClass()));

    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return;

    ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AICon->GetPawn());
    if (!Robot) return;

    if (Robot->CurrentState != ERobotState::Waiting) return;

    UE_LOG(LogTemp, Warning, TEXT("Call TickNode"));

    bool bFoundNextItem = false;
    bool bFoundAvailableCell = false;

    for (int32 i = 0; i < Robot->ProgressOfProductRecipe.RecipeData.Num(); ++i)
    {
        FProgressOfItem& Item = Robot->ProgressOfProductRecipe.RecipeData[i];
        if (Item.ProgressValue == 0) 
        {
            bFoundNextItem = true;

            if (Manager)
            {
                for (AItemProductionCell* Cell : Manager->ProductionCellList)
                {
                    if (!Cell) continue;

                    if (Cell->ProductProcessData == Item.ItemData &&
                        Cell->CurrentState == ECellProgressState::Empty)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("New TargetCell is %s"), *Cell->GetName());


                        // Robot에 TargetCell 세팅
                        Robot->TargetCell = Cell;
                        Robot->TargetPoint = Cell->GetActorLocation();
   
                        Robot->CurrentState = ERobotState::Moving;
                        Robot->SetTargetCell();
                        
                        Cell->CurrentState = ECellProgressState::Reserved;

                        Robot->SetTargetCell();
                        UE_LOG(LogTemp, Warning, TEXT("Robot %s assigned to Cell %s"),
                            *Robot->GetName(), *Cell->GetName());

                        OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetCellKey.SelectedKeyName, Cell);
                        return; 
                    }
                }
            }

            if (!bFoundAvailableCell)
            {
                UE_LOG(LogTemp, Warning, TEXT("%s : 다음 아이템은 있지만 Cell이 비어있지 않음. 대기 유지."), *Robot->GetName());
                Robot->CurrentState = ERobotState::Waiting;
                return;
            }
        }
    }


    if (Manager && Manager->ProductWarehousePoint)
    {
        Robot->TargetPoint = Manager->ProductWarehousePoint->GetActorLocation();
        Robot->TargetPoint.Z = FVector::ZeroVector.Z;
        Robot->SetTargetPoint();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ProductWarehousePoint is nullptr in %s"), *Manager->GetName());
        return;
    }

    Robot->CurrentState = ERobotState::Returning;
    Robot->SetRobotState();

    OwnerComp.GetBlackboardComponent()->ClearValue(TargetCellKey.SelectedKeyName);
}