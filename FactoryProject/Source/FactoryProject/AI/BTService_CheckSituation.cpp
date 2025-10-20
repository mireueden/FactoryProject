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

    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return;

    ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AICon->GetPawn());
    if (!Robot) return;

    if (Robot->CurrentState != ERobotState::Waiting) return;

    //if (Robot->TargetCell) return;
    
    // 상태가 Waiting일때, 다음 진행해야할 Cell과 Item등의 세팅을 해주는 Service라고 생각하자.


    UE_LOG(LogTemp, Warning, TEXT("Call TickNode"));

    bool bFoundNextItem = false;
    bool bFoundAvailableCell = false;

    for (int32 i = 0; i < Robot->ProgressOfProductRecipe.RecipeData.Num(); ++i)
    {
        FProgressOfItem& Item = Robot->ProgressOfProductRecipe.RecipeData[i];
        if (Item.ProgressValue == 0) // 아직 진행되지 않은 Item
        {
            bFoundNextItem = true;

            // Manager에서 Cell 리스트 참조
            if (ADeliveryRobotManager* Manager = Cast<ADeliveryRobotManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADeliveryRobotManager::StaticClass())))
            {
                for (AItemProductionCell* Cell : Manager->ProductionCellList)
                {
                    if (!Cell) continue;

                    // Cell이 비어 있고, Item이 동일하면 Target 설정
                    if (Cell->ProductProcessData == Item.ItemData &&
                        Cell->CurrnetState == ECellProgressState::Empty)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("New TargetCell is %s"), *Cell->GetName());


                        // Robot에 TargetCell 세팅
                        Robot->TargetCell = Cell;
                        Robot->TargetPoint = Cell->GetActorLocation();
   
                        Robot->CurrentState = ERobotState::Moving;
                        Robot->SetTargetCell();
                        // Cell 상태 Reserved // 다른 모든 Cell 까지 Reserved시킴.
                        // Robot이 확실하게 이동할 Cell만 Reservedㅈ하고, 저장하게 한다음에, Robot이 Complete상태가 되기 전에는 다시 갱신 안하게끔.
                        Cell->CurrnetState = ECellProgressState::Reserved;

                        Robot->SetTargetCell();
                        UE_LOG(LogTemp, Warning, TEXT("Robot %s assigned to Cell %s"),
                            *Robot->GetName(), *Cell->GetName());

                        OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetCellKey.SelectedKeyName, Cell);
                        return; // 첫 번째 가능한 Cell만 선택
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

    // for문 벗어나면 모든 recipe의 Progress가 1인것
    Robot->CurrentState = ERobotState::Returning;
    Robot->SetRobotState();

    // 진행 가능한 Cell 없으면 TargetCell 초기화
    OwnerComp.GetBlackboardComponent()->ClearValue(TargetCellKey.SelectedKeyName);

}