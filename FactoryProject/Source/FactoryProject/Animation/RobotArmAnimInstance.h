// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Object/RobotArm.h"
#include "RobotArmAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API URobotArmAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ARobotArm* RobotArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsGrabItemCheck = false;

};
