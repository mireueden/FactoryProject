// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/RobotArmAnimInstance.h"

void URobotArmAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	AActor* OwningActor = GetOwningActor();

	if (!OwningActor) return;

	//if (ARobotArm* Arm = Cast<ARobotArm>(OwningActor))
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("ARobotArm"));
	//	bIsGrabItemCheck = Arm->bIsGrabItem;
	//}
	//else if (ACellRobotArm* CellArm = Cast<ACellRobotArm>(CellRobotArm))
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("ACellRobotArm"));
	//	bIsGrabItemCheck = CellArm->bIsGrabedMesh;
	//}

	if (!OwningActor) return;

	RobotArm = Cast<ARobotArm>(OwningActor);
	if (RobotArm)
		bIsGrabItemCheck = RobotArm->bIsGrabItem;

	CellRobotArm = Cast<ACellRobotArm>(OwningActor);
	if (CellRobotArm)
		bIsGrabItemCheck = CellRobotArm->bIsGrabedMesh;
}
