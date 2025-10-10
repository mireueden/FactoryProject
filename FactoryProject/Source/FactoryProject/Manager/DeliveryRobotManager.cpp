// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DeliveryRobotManager.h"

#include "UObject/ConstructorHelpers.h"
#include "Object/ConveyorBelt.h"
#include "Object/item.h"

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
    RobotSpawnLocation = GetActorLocation();
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
             NewRobot->SetTargetItem(TargetItem);
         }
     }
     else
         UE_LOG(LogTemp, Log, TEXT("Robot can't Spawn"));

}

void ADeliveryRobotManager::DestoryDeliveryRobot()
{
}

