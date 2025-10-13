// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AI/DeliveryRobot.h"
#include "AI/DeliveryRobotController.h"

#include "DeliveryRobotManager.generated.h"

class AConveyorBelt;
class AItem;

UCLASS()
class FACTORYPROJECT_API ADeliveryRobotManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeliveryRobotManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Robots")
	TSubclassOf<ADeliveryRobot> DeliveryRobotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Robots")
	TSubclassOf<ADeliveryRobotController> DeliveryRobotControllerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conveyor")
	TArray<AConveyorBelt*> ConveyorBelts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robots")
	TArray<ADeliveryRobot*> DeliveryRobots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robots")
	FVector RobotSpawnLocation;


	UPROPERTY(VisibleAnywhere)
	USceneComponent* RobotSpawnPoint;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RobotReturnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robots")
	FRotator RobotSpawnRotate;

	UFUNCTION()
	void CheckArrived(AItem* ArrivedItem);

	UFUNCTION()
	void SetDeliveryRobot(AItem* TargetItem);

	UFUNCTION()
	void DestoryDeliveryRobot();
};
