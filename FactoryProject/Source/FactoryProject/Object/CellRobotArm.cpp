// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/CellRobotArm.h"

#include "Animation/RobotArmAnimInstance.h"


// Sets default values
ACellRobotArm::ACellRobotArm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void ACellRobotArm::BeginPlay()
{
	Super::BeginPlay();

}

void ACellRobotArm::PostInitializeComponents()
{
	Super::PostInitializeComponents();  

	if (URobotArmAnimInstance* Anim = Cast<URobotArmAnimInstance>(RobotArmComp->GetAnimInstance()))
	{
		// AnimInstance 내부에서 필요한 포인터나 상태 전달
		Anim->CellRobotArm = this;
	}
}
// Called every frame
void ACellRobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

