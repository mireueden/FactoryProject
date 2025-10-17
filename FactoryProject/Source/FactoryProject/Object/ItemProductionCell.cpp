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

	//  Collision 설정
	// "QueryOnly" → 물리 Block은 안 하지만, Overlap 이벤트는 발생 가능
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



	//NavModifier->SetAreaClassToReplace(UNavArea_AvoidCell::StaticClass());
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

	UE_LOG(LogTemp, Warning, TEXT("Call Func OnRobotEnterCell"));

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

void AItemProductionCell::OnDetectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Cell Overlap Begin: %s"), *OtherActor->GetName());
}

void AItemProductionCell::OnDetectEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Cell Overlap End: %s"), *OtherActor->GetName());
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

