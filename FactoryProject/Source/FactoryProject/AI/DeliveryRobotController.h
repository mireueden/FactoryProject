// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DeliveryRobotController.generated.h"

UENUM(BlueprintType)
enum class ERobotState : uint8
{
	None,
	Waiting,
	Moving,
	Delivery,
	Returning,
	Error,
};

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API ADeliveryRobotController : public AAIController
{
	GENERATED_BODY()
	

public:

	ADeliveryRobotController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	ERobotState CurrentState;




};
