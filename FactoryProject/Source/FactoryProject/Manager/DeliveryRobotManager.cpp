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


    TArray<AActor*> FoundCells;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemProductionCell::StaticClass(), FoundCells);

    for (AActor* Actor : FoundCells)
    {
        if (AItemProductionCell* Cell = Cast<AItemProductionCell>(Actor))
        {
            ProductionCellList.Add(Cell);
        }
    }
}

void ADeliveryRobotManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (AConveyorBelt* Belt : ConveyorBelts)
    {
        if (IsValid(Belt))
        {
            Belt->OnItemArrived.RemoveDynamic(this, &ADeliveryRobotManager::CheckArrived);
        }
    }

    OnDeliveryRobotUpdate.Clear();

    DeliveryRobots.Empty();
    ProductionCellList.Empty();
    ConveyorBelts.Empty();

    Super::EndPlay(EndPlayReason);
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

     NewRobot->GetMesh()->SetVisibility(false);
     NewRobot->GetMesh()->SetHiddenInGame(true);
     NewRobot->GetMesh()->SetComponentTickEnabled(false);
     //NewRobot->GetMesh()->SetSkeletalMesh(nullptr);

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

             NewRobot->CurrentProcess = ERobotProcess::Delivery;
             NewRobot->SetProcessState();

             NewRobot->CurrentState = ERobotState::Moving;
             NewRobot->TargetPoint = TargetItem->ConveyorBelt->RobotArrivePoint->GetComponentLocation();
             NewRobot->ReturnPoint = RobotReturnPoint->GetComponentLocation();;
             NewRobot->SetTargetItem(TargetItem);
             
             UE_LOG(LogTemp, Warning, TEXT("TargetLoc: %s"), *NewRobot->ReturnPoint.ToString());

             DeliveryRobots.Add(NewRobot);
             OnDeliveryRobotUpdate.Broadcast();
         }
     }
     else
         UE_LOG(LogTemp, Warning, TEXT("Robot can't Spawn"));

}

void ADeliveryRobotManager::DestoryDeliveryRobot(ADeliveryRobot* DestoryRobot)
{
    DeliveryRobots.Remove(DestoryRobot);

    OnDeliveryRobotUpdate.Broadcast();
}

void ADeliveryRobotManager::SetProductRobot(UProductRecipeDataAsset* ProductRecipe)
{
    UE_LOG(LogTemp, Warning, TEXT("Call Func SetProductRobot"));

    if (!DeliveryRobotClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DeliveryRobotClass can't Found"));
        return;
    }

    FVector SpawnLocation = GetActorLocation();
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
    

    // Recipe Setting & TargetCell Setting
    NewRobot->ProductRecipeSetting(ProductRecipe);

    for (auto& RecipeItem : NewRobot->ProgressOfProductRecipe.RecipeData)
        RecipeItem.ProgressValue = 0;

    UItemBasicDataAsset* FirstItem = NewRobot->ProgressOfProductRecipe.RecipeData[0].ItemData;



    //ItemManager->StorageList[0].ItemData;

 
    AItemProductionCell* TargetCell = nullptr;

    for (AItemProductionCell* Cell : ProductionCellList)
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

    if (NewRobot)
    {
        AAIController* AICon = GetWorld()->SpawnActor<ADeliveryRobotController>(
            DeliveryRobotControllerClass,
            SpawnLocation,
            SpawnRotation
        );

        if (AICon)
        {
            AICon->Possess(NewRobot);

            NewRobot->CurrentProcess = ERobotProcess::ProductProcess;
            NewRobot->SetProcessState();

            if (TargetCell)
            {
                // 목적지 지정 및 상태 변경
                NewRobot->TargetCell = TargetCell;

                NewRobot->TargetPoint = TargetCell->GetActorLocation();
                NewRobot->TargetPoint.Z = NewRobot->GetActorLocation().Z;
                //NewRobot->TargetPoint = RobotReturnPoint->GetComponentLocation();


                //NewRobot->ReturnPoint = RobotReturnPoint->GetComponentLocation();
                NewRobot->ReturnPoint = ProductReturnPoint->GetActorLocation();
                NewRobot->CurrentState = ERobotState::Moving;
                TargetCell->CurrentState = ECellProgressState::Reserved;
                NewRobot->SetTargetCell();

                UE_LOG(LogTemp, Warning, TEXT("Robot %s assigned to Cell %s"),
                    *NewRobot->GetName(),
                    *TargetCell->GetName());
            }
            else     // 만약 모든 Cell의 State가 Emtmy가 아닌 경우, AI 생성만 하고 대기 상태 전환.
            {
                // 이동 가능한 Cell이 없으면 대기 상태 유지
                NewRobot->CurrentState = ERobotState::Waiting;
                NewRobot->ReturnPoint = ProductReturnPoint->GetActorLocation();
                NewRobot->SetRobotState();

                UE_LOG(LogTemp, Warning, TEXT("No available Cell. Robot %s waiting."), *NewRobot->GetName());
            }
            DeliveryRobots.Add(NewRobot);

            OnDeliveryRobotUpdate.Broadcast();
        }
    }
}