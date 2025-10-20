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
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("ReturnPoint"), ReturnPoint); 
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
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("ReturnPoint"), ReturnPoint);
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("TargetPoint"), TargetPoint);
    }
}

void ADeliveryRobot::SetRobotState()
{
    AAIController* AICon = Cast<ADeliveryRobotController>(GetController());
    if (AICon && AICon->GetBlackboardComponent())
    {
        AICon->GetBlackboardComponent()->SetValueAsVector(TEXT("ReturnPoint"), ReturnPoint);
        AICon->GetBlackboardComponent()->SetValueAsEnum(TEXT("RobotState"), static_cast<uint8>(CurrentState));
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
