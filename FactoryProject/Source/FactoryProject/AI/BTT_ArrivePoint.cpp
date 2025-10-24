// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ArrivePoint.h"

#include "DeliveryRobot.h"
#include "DeliveryRobotController.h"

UBTT_ArrivePoint::UBTT_ArrivePoint()
{
	NodeName = TEXT("ArrivePoint");
	bNotifyTick = true; 
}

EBTNodeResult::Type UBTT_ArrivePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Call UBTT_ArrivePoint"));

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
		return EBTNodeResult::Failed;


	ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController->GetCharacter());

	if (!Robot)
		return EBTNodeResult::Failed;

	// 목표 지점 저장
	TargetLocation = Robot->TargetPoint;
	TargetLocation.Z = Robot->GetActorLocation().Z;
	return EBTNodeResult::InProgress; // Tick에서 이동 처리


	//Robot->CurrentState = ERobotState::Arrived;
	//Robot->SetRobotState();

	//UE_LOG(LogTemp, Warning, TEXT("Robot %s 도착 → State: Arrived"), *Robot->GetName());

	//return EBTNodeResult::Succeeded;
}

void UBTT_ArrivePoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController ? AIController->GetPawn() : nullptr);
	if (!Robot) return;

	const float Distance = FVector::Dist(Robot->GetActorLocation(), TargetLocation);

	// 목표 지점으로 부드럽게 보정 이동
	if (Distance > 2.0f) // 2cm 이상 떨어져 있으면 보정
	{
		FVector Direction = (TargetLocation - Robot->GetActorLocation()).GetSafeNormal();
		FVector NewLocation = Robot->GetActorLocation() + Direction * 100.f * DeltaSeconds; // 100cm/s 속도

		Robot->SetActorLocation(NewLocation);
	}
	else
	{
		// 최종 위치 고정
		Robot->SetActorLocation(TargetLocation);

		Robot->CurrentState = ERobotState::Arrived;
		Robot->SetRobotState();


		UE_LOG(LogTemp, Warning, TEXT("Robot %s 정확히 도착 완료"), *Robot->GetName());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
