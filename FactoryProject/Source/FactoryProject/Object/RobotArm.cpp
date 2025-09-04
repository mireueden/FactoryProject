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
	bool bCheckItem = false;
	for (int i = 0; i < CheckItemData.Num(); i++)
	{
		if (Item->ItemData == CheckItemData[i])
			bCheckItem = true;
	}

	if (bCheckItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checked ITem"));
		switch (Item->ItemData->ItemIndex)
		{
		case 0:		case 4:
			ItemRotateValue = 90.0f;
			break;
		case 1:		case 5:
			ItemRotateValue = 150.0f;
			break;
		case 2:		case 6:
			ItemRotateValue = -150.0f;
			break;
		case 3:		case 7:
			ItemRotateValue = -90.0f;
			break;
		}
		UE_LOG(LogTemp, Warning, TEXT("ItemRotateValue : %f"), ItemRotateValue);

		GrabedItem = Item;

		GrabedItem->SplineProgress = 0.f;
		GrabedItem->MoveSpeed = 0.f;
		GrabedItem->SplineComp = NULL;
		Item->ItemState = EItemState::Grabed;

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
		Item->MoveSpeed = 100.f;
		Item->ItemState = EItemState::Move;
	}
}

