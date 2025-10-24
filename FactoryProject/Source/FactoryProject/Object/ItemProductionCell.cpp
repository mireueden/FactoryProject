// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ItemProductionCell.h"


#include "AI/DeliveryRobot.h"

// Sets default values
AItemProductionCell::AItemProductionCell()
{

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

	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(OtherActor);


	if (Robot->TargetCell != this) return;

	ProcessRobot = Robot;



	// // cell 이 resered상태일때, 해당하는 Robot이 들어오면?
	//UE_LOG(LogTemp, Warning, TEXT("Call Func OnRobotEnterCell"));

	//ADeliveryRobot* Robot = Cast<ADeliveryRobot>(OtherActor);
	//if (!Robot) return;

	//ProcessRobot = Robot;

	//if (ProcessRobot != nullptr) return;

	//if (Robot->TargetCell != this) return;

	//if (Robot->CurrentState != ERobotState::Arrived) {
	//	UE_LOG(LogTemp, Warning, TEXT("%s : 아직 도착 안 함, 무시"), *Robot->GetName());
	//	return;
	//}

	//CellStateChanged(ECellProgressState::InProgress);

	//Robot->CurrentState = ERobotState::Working;
	//UE_LOG(LogTemp, Warning, TEXT("%s 시작됨 by %s"), *GetName(), *Robot->GetName());

	//CraftingProduct(); // 임시 공정 완료 함수
}

void AItemProductionCell::OnRobotExitCell(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Call Func OnRobotExitCell"));

	ADeliveryRobot* Robot = Cast<ADeliveryRobot>(OtherActor);

	if (!Robot) return;

	if (Robot != BeforeProcessRobot || Robot->CurrentState == ERobotState::Waiting) return;

	UE_LOG(LogTemp, Warning, TEXT("Robot == BeforeProcessRobot"));

	BeforeProcessRobot = Robot;

	ProcessRobot = nullptr;


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

void AItemProductionCell::ProductionProcess()
{

	// Wheel
	if (ProductProcessData->ItemMeshes.Num() == 1) // Wheel은 따로 Attach하지 않음
	{
		ProcessRobot->GetMesh()->UnHideBoneByName(TEXT("Mz3_Wheel_Left_Back"));
		ProcessRobot->GetMesh()->UnHideBoneByName(TEXT("Mz3_Wheel_Left_Front"));
		ProcessRobot->GetMesh()->UnHideBoneByName(TEXT("Mz3_Wheel_Right_Back"));
		ProcessRobot->GetMesh()->UnHideBoneByName(TEXT("Mz3_Wheel_Right_Front"));
		return;
	}

	// Paint
	if (ProductProcessData->ItemMeshes.Num() == 0 && ProductProcessData->ItemMaterial)
	{
		int32 SlotIndex = 0; // 0번 : 몸체 Material
		ProcessRobot->GetMesh()->SetMaterial(SlotIndex, ProductProcessData->ItemMaterial);

		for (int i = 0; i < ProcessRobot->AttachedParts.Num(); i++)
		{
			ProcessRobot->AttachedParts[i].PartComponent->SetMaterial(
				ProcessRobot->AttachedParts[i].PaintSlotIndex,
				ProductProcessData->ItemMaterial);
		}

		//for (UStaticMeshComponent* PartComp : ProcessRobot->AttachedParts)
		//{
		//	if (PartComp)
		//	{
		//		PartComp->SetMaterial(0, ProductProcessData->ItemMaterial);
		//	}
		//}

		UE_LOG(LogTemp, Log, TEXT("[Painting] Applied new material: %s"),
			*ProductProcessData->ItemMaterial->GetName());
		return;
	}


	// Attach
	if (ProductProcessData->ItemMeshes.Num() > 0)
	{
		for (const FItemMeshAttachData& MeshData : ProductProcessData->ItemMeshes)
		{
			if (!MeshData.ItemMeshes) continue;

			FName TargetSocket = MeshData.TargetSocket;
			if (!ProcessRobot->GetMesh()->DoesSocketExist(TargetSocket))
			{
				UE_LOG(LogTemp, Warning, TEXT("Socket not found: %s"), *TargetSocket.ToString());
				continue;
			}

			UStaticMeshComponent* PartComp = NewObject<UStaticMeshComponent>(ProcessRobot);
			PartComp->RegisterComponent();
			PartComp->SetStaticMesh(MeshData.ItemMeshes);

			PartComp->AttachToComponent(
				ProcessRobot->GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				TargetSocket
			);

			FAttachedPartInfo Info;
			Info.PartComponent = PartComp;
			Info.PaintSlotIndex = MeshData.PaintSlotIndex;
			ProcessRobot->AttachedParts.Add(Info);
		}
		return;
	}

}

void AItemProductionCell::CraftingProduct()
{
	if (!ProcessRobot) return;

	CellStateChanged(ECellProgressState::InProgress);
	// 제작공정 


	if (CurrentState != ECellProgressState::InProgress || ProcessRobot->CurrentState != ERobotState::Working)
		return;


	// 임시 공정 
	ProductionProcess();


	//ProductProcessData->ItemMeshes[0]->AttachToComponent(
	//	ProcessRobot->GetMesh(),
	//	FAttachmentTransformRules::SnapToTargetIncludingScale,
	//	TEXT("SocketName")
	//);

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

