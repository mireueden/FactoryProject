// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/RobotArm.h"

// Sets default values
ARobotArm::ARobotArm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	RobotStandComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RobotStandComp"));
	RobotStandComp->SetupAttachment(RootComponent);

	RobotArmComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotArmhComp"));
	RobotArmComp->SetupAttachment(RobotStandComp);

	StandBoxComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StandBoxComp"));
	StandBoxComp->SetupAttachment(RootComponent);

	CheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckBox"));
	CheckBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARobotArm::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Overlap 이벤트에서 실행
void ARobotArm::CheckItem(AItem* Item)
{
	// 해당 로봇팔이 분류 시도할 아이템인지
	if (Item->ItemData == CheckItemData)
	{
		GrabedItem = Item;
		//CurrentConveyorBelt->UnRegisterItem(Item);

		GrabedItem->MoveSpeed = 0.f;
		GrabedItem->SplineComp = NULL;
		GrabedItem->ItemState = EItemState::Stop;

		GrabItem(GrabedItem);
	}
}

void ARobotArm::GrabItem(AItem* Item)
{
	if (Item && RobotArmComp)
	{
		bIsGrabItem = true;

		Item->AttachToComponent(
			RobotArmComp,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("Brush_endSocket")
		);
	}

}

void ARobotArm::SpreadItem(AItem* Item)
{
	if (Item && RobotArmComp)
	{
		bIsGrabItem = false;
		Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

