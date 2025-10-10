// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "DeliveryRobot.generated.h"

UENUM(BlueprintType)
enum class ERobotState : uint8
{
	None,
	Moving,
	Waiting,
	Delivery,
	Returning,
	Error,
};


class AItem;
class ADeliveryRobotController;

UCLASS()
class FACTORYPROJECT_API ADeliveryRobot : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeliveryRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RobotRootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<ADeliveryRobotController> DeliveryRobotAIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	ERobotState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	AItem* TargetItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector TargetPoint;

	UFUNCTION()
	void SetTargetItem(AItem* Item);

	UFUNCTION()
	void AttachItem();

	UFUNCTION()
	void DetachItem();
};
