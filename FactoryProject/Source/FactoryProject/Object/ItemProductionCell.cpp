// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ItemProductionCell.h"


#include "AI/DeliveryRobot.h"

// Sets default values
AItemProductionCell::AItemProductionCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	DetectArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectArea"));
	DetectArea->SetupAttachment(RootComponent);
	DetectArea->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void AItemProductionCell::BeginPlay()
{
	Super::BeginPlay();

	DetectArea->OnComponentBeginOverlap.AddDynamic(this, &AItemProductionCell::OnRobotEnterCell);

}

// Called every frame
void AItemProductionCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemProductionCell::OnRobotEnterCell(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(OtherActor);
	if (!Robot) return;

	if (ProcessRobot != nullptr) return;

	if (Robot->TargetCell != this) return;

	ProcessRobot = Robot;
	CurrnetState = ECellProgressState::InProgress;
	Robot->CurrentState = ERobotState::Working;
	UE_LOG(LogTemp, Warning, TEXT("%s 시작됨 by %s"), *GetName(), *Robot->GetName());

	// 공정 시작 (예: 일정 시간 뒤 Complete)
	//GetWorldTimerManager().SetTimer(ProcessTimer, this, &AItemProductionCell::CompleteProduction, ProcessTime, false);
}

void AItemProductionCell::CellStateChanged(ECellProgressState NewState)
{
	if (CurrnetState != NewState)
	{
		CurrnetState = NewState;
		OnCellStateChanged.Broadcast(NewState);
	}
}

void AItemProductionCell::CraftingProduct()
{
	if (!ProcessRobot) return;

	CurrnetState = ECellProgressState::Completed;
	ProcessRobot->CurrentState = ERobotState::Waiting; // 다시 대기 상태로 전환
	ProcessRobot->TargetCell = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("%s 공정 완료"), *GetName());

	ProcessRobot = nullptr; // Cell에서 로봇 해제
}

