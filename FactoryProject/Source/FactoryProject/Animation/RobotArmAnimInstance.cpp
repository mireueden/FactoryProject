// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/RobotArmAnimInstance.h"

void URobotArmAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	AActor* OwningActor = GetOwningActor();
	if (!OwningActor) return;

	RobotArm = Cast<ARobotArm>(OwningActor);
	if (!RobotArm) return;



	bIsGrabItemCheck = RobotArm->bIsGrabItem;
}
