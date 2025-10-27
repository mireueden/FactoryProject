// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ItemProductionCell.h"
#include "Animation/RobotArmAnimInstance.h"


#include "AI/DeliveryRobot.h"

// Sets default values
AItemProductionCell::AItemProductionCell()
{

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	MainStaticComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainComp"));
	MainStaticComp->SetupAttachment(RootScene);

	RobotFloorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RobotFloorComp"));
	RobotFloorComp->SetupAttachment(MainStaticComp);

	LeftStandMatComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftStandMatComp"));
	LeftStandMatComp->SetupAttachment(MainStaticComp);
	RightStandMatComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightStandMatComp"));
	RightStandMatComp->SetupAttachment(MainStaticComp);






	DetectArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectArea"));
	DetectArea->SetupAttachment(RootComponent);

	DetectArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // AI나 Player 감지용

	DetectArea->SetCanEverAffectNavigation(true);
	DetectArea->SetAreaClassOverride(UNavArea_AvoidCell::StaticClass());

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


	if (ArmActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FRotator LeftRotator = FRotator::ZeroRotator;
		LeftRotator.Yaw += 180.f;
		// 왼쪽
		LeftArmActor = GetWorld()->SpawnActor<AActor>(
			ArmActorClass,
			FVector::ZeroVector,
			LeftRotator,
			SpawnParams
		);
		if (LeftArmActor)
		{
			LeftArmActor->AttachToComponent(LeftStandMatComp, FAttachmentTransformRules::KeepRelativeTransform);
			LeftArmActor->SetActorRelativeLocation(FVector(0, 0, 0)); // 위치 조정
		}

		// 오른쪽
		RightArmActor = GetWorld()->SpawnActor<AActor>(
			ArmActorClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		if (RightArmActor)
		{
			RightArmActor->AttachToComponent(RightStandMatComp, FAttachmentTransformRules::KeepRelativeTransform);
			RightArmActor->SetActorRelativeLocation(FVector(0, 0, 0));
		}
	}


	//NavModifier->SetAreaClassToReplace(UNavArea_AvoidCell::StaticClass());
}

// Called every frame
void AItemProductionCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (ProcessRobot->CurrentState == ERobotState::Working)
	//	CraftingProduct();
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
	UE_LOG(LogTemp, Warning, TEXT("%s 공정 시작"), *GetName());
	if (!ProcessRobot) return;

	CellStateChanged(ECellProgressState::InProgress);
 


	if (CurrentState != ECellProgressState::InProgress || ProcessRobot->CurrentState != ERobotState::Working)
		return;


	// 제작 공정 
	ProductionProcess();


	//ProductProcessData->ItemMeshes[0]->AttachToComponent(
	//	ProcessRobot->GetMesh(),
	//	FAttachmentTransformRules::SnapToTargetIncludingScale,
	//	TEXT("SocketName")
	//);


	// 이관할 기능 
	// =====
	CurrentState = ECellProgressState::Completed;
	ProcessRobot->CurrentState = ERobotState::Waiting; // 다시 대기 상태로 전환

	ProcessRobot->SetRobotState();


	for (FProgressOfItem& RecipeItem : ProcessRobot->ProgressOfProductRecipe.RecipeData)
	{
		if (RecipeItem.ItemData == ProductProcessData)
		{
			RecipeItem.ProgressValue = 1;
			break;
		}
	}

	ProcessRobot->TargetCell = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("%s 공정 완료"), *GetName());

	BeforeProcessRobot = ProcessRobot;
	ProcessRobot = nullptr; // Cell에서 로봇 해제

	// ====

}

void AItemProductionCell::ReadyToCraftingProduct()
{
	// robot에 각 할당된 Item의 StaticMesh Attach 
	// robot의 애니메이션 재생에 필요한 값 변경으로 애니메이션 재생 시작.
	
	for (int i = 0; i < 2; i++)
	{
		//CellRobotArmList[i]->bIsGrabItem = true;

		ProductProcessData->ItemMeshes[i].ItemMeshes;

		// 소켓이름 Brush_endSocket
		// 해당 소켓 위치에 spawn
		// StaticMesh 해당 소켓에 Attach


		if (!ProductProcessData->ItemMeshes.IsValidIndex(i)) continue;
		UStaticMesh* TargetMesh = ProductProcessData->ItemMeshes[i].ItemMeshes;
		if (!TargetMesh) continue;

		const FName SocketName = TEXT("Brush_endSocket");

		//USkeletalMeshComponent* ArmMesh = CellRobotArmList[i]; // 또는 LeftRobotArmComp, RightRobotArmComp 등
		//if (!ArmMesh) continue;

		//const FTransform SocketTransform = ArmMesh->GetSocketTransform(SocketName, RTS_World);



	}








	// 애니메이션의 재생이 완료시 호출되거나 실행 되어야 하는 기능
	// Robot Floor Comp의 회전 및 Z축 위치상승 원위치.
	// Robot Floor Comp에서 Robot Detach 


	// 위의 행동 종료 후 [이관할 기능] 실행하기.
}

