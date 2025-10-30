// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/CellRobotArm.h"

#include "Animation/RobotArmAnimInstance.h"


ACellRobotArm::ACellRobotArm()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	PlatformComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformComp"));
	PlatformComp->SetupAttachment(RootScene);

	RobotStandComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RobotStandComp"));
	RobotStandComp->SetupAttachment(PlatformComp);

	RobotArmComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotArmComp"));
	RobotArmComp->SetupAttachment(RobotStandComp);


	RobotArmComp->SetAnimInstanceClass(ArmAnimClass);
}

void ACellRobotArm::BeginPlay()
{
	Super::BeginPlay();
}

void ACellRobotArm::PostInitializeComponents()
{
	Super::PostInitializeComponents();  

	if (URobotArmAnimInstance* Anim = Cast<URobotArmAnimInstance>(RobotArmComp->GetAnimInstance()))
	{
		Anim->CellRobotArm = this;
	}
}

void ACellRobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AItemProductionCell* ACellRobotArm::GetOwnerCell()
{
	return Cast<AItemProductionCell>(GetOwner());
}

void ACellRobotArm::ProductRequest()
{
	OnRequestProductProcessDone.Broadcast();

	bIsGrabedMesh = false;
	bIsProcess = false;
}
