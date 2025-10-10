// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DeliveryRobot.h"

#include "DeliveryRobotController.h"
#include "AIController.h"
#include "Object/Item.h"

// Sets default values
ADeliveryRobot::ADeliveryRobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CurrentState = ERobotState::None;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    AIControllerClass = ADeliveryRobotController::StaticClass();


    RobotRootComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RobotRootComp"));
    RobotRootComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADeliveryRobot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeliveryRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeliveryRobot::SetTargetItem(AItem* Item)
{
    TargetItem = Item;

    AAIController* AICon = Cast<ADeliveryRobotController>(GetController());
    if (AICon && AICon->GetBlackboardComponent())
    {
        //FVector ItemLocation = TargetPoint->GetActorLocation();

        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("TargetItem"), TargetPoint);
        //AICon->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetItem"), TargetItem);
        AICon->GetBlackboardComponent()->SetValueAsEnum(TEXT("RobotState"), static_cast<uint8>(CurrentState));

    }
}

void ADeliveryRobot::AttachItem()
{
    TargetItem->AttachToComponent(
        RobotRootComp,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        FName("Brush_endSocket")
    );

}

void ADeliveryRobot::DetachItem()
{
    TargetItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}