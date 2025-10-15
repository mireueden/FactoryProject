// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DeliveryRobotManager.h"

#include "UObject/ConstructorHelpers.h"
#include "Object/ConveyorBelt.h"
#include "Object/item.h"
#include "Widget/ProductOrderManagementWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
ADeliveryRobotManager::ADeliveryRobotManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<ADeliveryRobot> DeliveryRobotClassFinder(
        TEXT("/Game/Project/Blueprint/BP_DeliveryRobot.BP_DeliveryRobot_C"));
    if (DeliveryRobotClassFinder.Succeeded())
        DeliveryRobotClass = DeliveryRobotClassFinder.Class;

    static ConstructorHelpers::FClassFinder<ADeliveryRobotController> DeliveryRobotControllerClassFinder(
        TEXT("/Game/Project/Blueprint/BP_DeliveryRobotController.BP_DeliveryRobotController_C"));
    if (DeliveryRobotControllerClassFinder.Succeeded())
        DeliveryRobotControllerClass = DeliveryRobotControllerClassFinder.Class;

    RobotSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RobotSpawnPoint"));
    RootComponent = RobotSpawnPoint;

    RobotReturnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RobotReturnPoint"));
    RobotReturnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADeliveryRobotManager::BeginPlay()
{
	Super::BeginPlay();
	
    for (AConveyorBelt* Belt : ConveyorBelts)
    {
        if (Belt)
        {
            Belt->OnItemArrived.AddDynamic(this, &ADeliveryRobotManager::CheckArrived);
        }
    }
}

// Called every frame
void ADeliveryRobotManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeliveryRobotManager::CheckArrived(AItem* ArrivedItem)
{
    UE_LOG(LogTemp, Warning, TEXT("Call Func CheckArrived"));
    SetDeliveryRobot(ArrivedItem);
}

void ADeliveryRobotManager::SetDeliveryRobot(AItem* TargetItem)
{
    if (!DeliveryRobotClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DeliveryRobotClass can't Found"));
        return;
    }

    // spawn DeliveryRobots
    RobotReturnPoint->GetComponentLocation();
    RobotSpawnRotate = GetActorRotation();

     const FVector SpawnLocation = GetActorLocation();
     const FRotator SpawnRotation = GetActorRotation();

     FActorSpawnParameters SpawnParams;
     SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
     SpawnParams.Owner = this;

     ADeliveryRobot* NewRobot = GetWorld()->SpawnActor<ADeliveryRobot>(
         DeliveryRobotClass,  
         SpawnLocation,
         SpawnRotation,
         SpawnParams
     );
     
     if (NewRobot)
     {
         // AIController 생성
             AAIController* AICon = GetWorld()->SpawnActor<ADeliveryRobotController>(
                 DeliveryRobotControllerClass,
                SpawnLocation,
                SpawnRotation
            );

         if (AICon)
         {
             AICon->Possess(NewRobot);

             NewRobot->CurrentState = ERobotState::Moving;
             NewRobot->TargetPoint = TargetItem->ConveyorBelt->RobotArrivePoint->GetComponentLocation();
             NewRobot->ReturnPoint = RobotReturnPoint->GetComponentLocation();;
             NewRobot->SetTargetItem(TargetItem);
             
             UE_LOG(LogTemp, Warning, TEXT("TargetLoc: %s"), *NewRobot->ReturnPoint.ToString());

         }
     }
     else
         UE_LOG(LogTemp, Warning, TEXT("Robot can't Spawn"));

}

void ADeliveryRobotManager::DestoryDeliveryRobot()
{
}


void ADeliveryRobotManager::SetProductRobot(UProductRecipeDataAsset* ProductRecipe)
{
    UE_LOG(LogTemp, Warning, TEXT("Call Func SetProductRobot"));

    // 로봇 생성 및 recipe 세팅하기
    //ADeliveryRobot* NewRobot = GetWorld()->SpawnActor<ADeliveryRobot>(RobotClass, SpawnLocation, SpawnRotation);
    //if (NewRobot)
    //{
    //    NewRobot->ProductRecipeSetting(ProductRecipe)
    //}


    // recipe에 따른 이동할 목표 cell 찾기
    
    // 각각 해당 item의 제작 공정을 진행하는 목표 셀의 위치를 미리 다 저장하기? or 도착할때마다 찾기?

    // 

}