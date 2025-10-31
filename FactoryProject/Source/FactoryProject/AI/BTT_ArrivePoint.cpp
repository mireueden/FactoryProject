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

	TargetLocation = Robot->TargetPoint;
	//TargetLocation.Z = Robot->GetActorLocation().Z;

	CachedDirection = (TargetLocation - Robot->GetActorLocation()).GetSafeNormal();


	return EBTNodeResult::InProgress; 
}

void UBTT_ArrivePoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ADeliveryRobotController* AIController = Cast<ADeliveryRobotController>(OwnerComp.GetAIOwner());
	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(AIController ? AIController->GetPawn() : nullptr);
	if (!Robot) return;

	const float Distance = FVector::Dist(Robot->GetActorLocation(), TargetLocation);

	if (Distance > 20.0f) 
	{
		FVector NewLocation = Robot->GetActorLocation() + CachedDirection * 100.f * DeltaSeconds; // 100cm/s 속도

		Robot->SetActorLocation(NewLocation);
	}
	else
	{
		Robot->SetActorLocation(TargetLocation);

		Robot->ArrivedCell();

		UE_LOG(LogTemp, Warning, TEXT("Robot %s 정확히 도착 완료"), *Robot->GetName());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
