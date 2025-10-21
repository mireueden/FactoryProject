// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ItemProductionCell.h"


#include "AI/DeliveryRobot.h"

// Sets default values
AItemProductionCell::AItemProductionCell()
{
 //	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	//RootComponent = RootScene;

	//DetectArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectArea"));
	//DetectArea->SetupAttachment(RootComponent);
	//DetectArea->SetCollisionProfileName(TEXT("BlockAll"));
	//DetectArea->SetCanEverAffectNavigation(true);
	//DetectArea->SetAreaClassOverride(UNavArea_AvoidCell::StaticClass());

	//NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
	//NavModifier->AreaClass = UNavArea_AvoidCell::StaticClass();

	//NavModifier->FailsafeExtent = FVector(100.f, 100.f, 50.f); // Box 크기

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	// 감지 영역
	DetectArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectArea"));
	DetectArea->SetupAttachment(RootComponent);

	// Collision 설정
	// QueryOnly물리 Block은 안 하지만, Overlap 이벤트는 발생 가능
	DetectArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // AI나 Player 감지용

	//  Navigation 관련 설정
	// 이 컴포넌트가 NavMesh에 영향을 줄 수 있도록 함
	DetectArea->SetCanEverAffectNavigation(true);
	// NavMesh 상에서 이 영역은 AvoidCell로 처리되도록 지정
	DetectArea->SetAreaClassOverride(UNavArea_AvoidCell::StaticClass());

	//  NavModifierComponent로 NavMesh 반영 보조
	NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
	NavModifier->AreaClass = UNavArea_AvoidCell::StaticClass();
	NavModifier->FailsafeExtent = FVector(100.f, 100.f, 50.f);

}

// Called when the game starts or when spawned
void AItemProductionCell::BeginPlay()
{
	Super::BeginPlay();

	DetectArea->OnComponentBeginOverlap.AddDynamic(this, &AItemProductionCell::OnRobotEnterCell);
	DetectArea->OnComponentEndOverlap.AddDynamic(this, &AItemProductionCell::OnRobotExitCell);

	CurrentState = ECellProgressState::Empty;

	//NavModifier->SetAreaClassToReplace(UNavArea_AvoidCell::StaticClass());
}

// Called every frame
void AItemProductionCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProcessRobot->CurrentState == ERobotState::Working)
		CraftingProduct();
}

void AItemProductionCell::OnRobotEnterCell(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	
	UE_LOG(LogTemp, Warning, TEXT("Call Func OnRobotEnterCell"));

	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(OtherActor);
	if (!Robot) return;
	ProcessRobot = Robot;

	if (ProcessRobot != nullptr) return;

	if (Robot->TargetCell != this) return;

	if (Robot->CurrentState != ERobotState::Arrived) {
		UE_LOG(LogTemp, Warning, TEXT("%s : 아직 도착 안 함, 무시"), *Robot->GetName());
		return;
	}

	ProcessRobot = Robot;


	CellStateChanged(ECellProgressState::InProgress);

	Robot->CurrentState = ERobotState::Working;
	UE_LOG(LogTemp, Warning, TEXT("%s 시작됨 by %s"), *GetName(), *Robot->GetName());

	CraftingProduct(); // 임시 공정 완료 함수


	// 공정 시작 (예: 일정 시간 뒤 Complete)
	//GetWorldTimerManager().SetTimer(ProcessTimer, this, &AItemProductionCell::CompleteProduction, ProcessTime, false);
}

void AItemProductionCell::OnRobotExitCell(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Call Func OnRobotExitCell"));

	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(OtherActor);

	if (!Robot) return;

	if (Robot != BeforeProcessRobot || Robot->CurrentState == ERobotState::Waiting) return;

	UE_LOG(LogTemp, Warning, TEXT("Robot == BeforeProcessRobot"));

	BeforeProcessRobot = nullptr;

	if(CurrentState != ECellProgressState::Empty)
		CellStateChanged(ECellProgressState::Empty);
}


void AItemProductionCell::CellStateChanged(ECellProgressState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		OnCellStateChanged.Broadcast();
	}
}

void AItemProductionCell::CraftingProduct()
{
	if (!ProcessRobot) return;

	CellStateChanged(ECellProgressState::InProgress);
	// 제작공정 


	if (CurrentState != ECellProgressState::InProgress || ProcessRobot->CurrentState != ERobotState::Working)
		return;

	CurrentState = ECellProgressState::Completed;
	ProcessRobot->CurrentState = ERobotState::Waiting; // 다시 대기 상태로 전환

	ProcessRobot->SetRobotState();
	// ==

	for (FProgressOfItem& RecipeItem : ProcessRobot->ProgressOfProductRecipe.RecipeData)
	{
		if (RecipeItem.ItemData == ProductProcessData)
		{
			RecipeItem.ProgressValue = 1;
			break;
		}
	}

	// ==

	ProcessRobot->TargetCell = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("%s 공정 완료"), *GetName());

	BeforeProcessRobot = ProcessRobot;
	ProcessRobot = nullptr; // Cell에서 로봇 해제
}

