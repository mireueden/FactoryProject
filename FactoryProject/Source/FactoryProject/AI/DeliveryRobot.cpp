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
    CurrentProcess = ERobotProcess::None;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    AIControllerClass = ADeliveryRobotController::StaticClass();


    RobotRootComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RobotRootComp"));
    RobotRootComp->SetupAttachment(RootComponent);

    GetMesh()->SetupAttachment(RobotRootComp);
}

// Called when the game starts or when spawned
void ADeliveryRobot::BeginPlay()
{
	Super::BeginPlay();

    // 공정을 위한 Wheel Bone 모두 숨기기
    GetMesh()->HideBoneByName(TEXT("Mz3_Wheel_Left_Back"), EPhysBodyOp::PBO_None);
    GetMesh()->HideBoneByName(TEXT("Mz3_Wheel_Left_Front"), EPhysBodyOp::PBO_None);
    GetMesh()->HideBoneByName(TEXT("Mz3_Wheel_Right_Back"), EPhysBodyOp::PBO_None);
    GetMesh()->HideBoneByName(TEXT("Mz3_Wheel_Right_Front"), EPhysBodyOp::PBO_None);
}

// Called every frame
void ADeliveryRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeliveryRobot::SetProcessState()
{
    AAIController* AICon = Cast<ADeliveryRobotController>(GetController());
    if (AICon && AICon->GetBlackboardComponent())
    {
        AICon->GetBlackboardComponent()->SetValueAsEnum(TEXT("RobotProcessState"), static_cast<uint8>(CurrentProcess));

    }
}

void ADeliveryRobot::SetTargetCell()
{
    AAIController* AICon = Cast<ADeliveryRobotController>(GetController());
    if (AICon && AICon->GetBlackboardComponent())
    {
        AICon->GetBlackboardComponent()->SetValueAsEnum(TEXT("RobotState"), static_cast<uint8>(CurrentState));
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("TargetPoint"), TargetPoint);
        AICon->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetCell"), TargetCell);
    }
}

void ADeliveryRobot::SetTargetItem(AItem* Item)
{
    TargetItem = Item;

    AAIController* AICon = Cast<ADeliveryRobotController>(GetController());
    if (AICon && AICon->GetBlackboardComponent())
    {
        AICon->GetBlackboardComponent()->SetValueAsEnum(TEXT("RobotState"), static_cast<uint8>(CurrentState));
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("TargetPoint"), TargetPoint);
    }
}

void ADeliveryRobot::SetRobotState()
{
    AAIController* AICon = Cast<ADeliveryRobotController>(GetController());
    if (AICon && AICon->GetBlackboardComponent())
    {
        AICon->GetBlackboardComponent()->SetValueAsEnum(TEXT("RobotState"), static_cast<uint8>(CurrentState));
    }
}

void ADeliveryRobot::SetTargetPoint()
{
    AAIController* AICon = Cast<ADeliveryRobotController>(GetController());
    if (AICon && AICon->GetBlackboardComponent())
    {
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("TargetPoint"), TargetPoint);
    }
}

void ADeliveryRobot::SetStopoverPoint()
{
    AAIController* AICon = Cast<ADeliveryRobotController>(GetController());
    if (AICon && AICon->GetBlackboardComponent())
    {
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("ReturnPoint"), ReturnPoint);
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("StoragePoint"), StoragePoint);
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("ReturnRoutePoint"), ReturnRoutePoint);
    }
}

void ADeliveryRobot::AttachItem()
{
    TargetItem->AttachToComponent(
        RobotRootComp,
        FAttachmentTransformRules::KeepRelativeTransform
    );
    TargetItem->SetActorRelativeLocation(FVector(0, 0, 150));
}

void ADeliveryRobot::DetachItem()
{
    TargetItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ADeliveryRobot::ProductRecipeSetting(UProductRecipeDataAsset* ProductRecipe)
{
    ProductRecipeData = ProductRecipe;

    ProgressOfProductRecipe.ProductName = ProductRecipe->ProductName;
    ProgressOfProductRecipe.RecipeData.Empty(); // 기존 데이터 초기화

    for (int i = 0; i < ProductRecipe->RecipeData.Num(); i++)
    {
        FProgressOfItem NewItem;
        NewItem.ItemData = ProductRecipe->RecipeData[i].ItemData;
        NewItem.ProgressValue = 0;

        ProgressOfProductRecipe.RecipeData.Add(NewItem);
    }
}

void ADeliveryRobot::ArrivedCell()
{
    if (TargetCell)
    {
        AttachToComponent(
            TargetCell->RobotFloorComp,
            FAttachmentTransformRules::SnapToTargetNotIncludingScale
        );
        //TargetItem->SetActorRelativeLocation(FVector(0, 0, 150));
        
        SetActorRelativeRotation(FRotator::ZeroRotator);
        FVector Loc = FVector::ZeroVector;
        Loc.Z += 20.0f;
        SetActorRelativeLocation(Loc);

        TargetCell->OnFloorMoveFinished.AddDynamic(this, &ADeliveryRobot::ProcessCompletionInRobot);
        TargetCell->FloorMove();

    }

    CurrentState = ERobotState::Arrived;
    SetRobotState();
}

void ADeliveryRobot::ProcessCompletionInRobot()
{
    // 호출되는 타이밍이 늦어서 readyToWork가 실행이 되고 나서 호출됨 -> 그래서 공정 진행 안하고 곧바로 종료함.
    if (CurrentState != ERobotState::Working) // 이게 의미없는 느낌-> 그럼 처음부분에서는 어떻게 수정하지?
        return;

    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    if (TargetCell)
    {
        TargetCell->OnFloorMoveFinished.RemoveDynamic(this, &ADeliveryRobot::ProcessCompletionInRobot);
    }

    TargetCell->ProcessCompletionInCell();
}
