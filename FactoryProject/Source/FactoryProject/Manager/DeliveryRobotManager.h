
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AI/DeliveryRobot.h"
#include "AI/DeliveryRobotController.h"
#include "DataAsset/ItemManagerActor.h"

#include "Object/ItemProductionCell.h"

#include "DeliveryRobotManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeliveryRobotUpdate);


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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
	USceneComponent* ProductStoragePoint;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ItemStoragePoint;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RobotReturnRoutePoint;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RobotReturnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robots")
	AActor* ProductWarehousePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robots")
	FRotator RobotSpawnRotate;

	UFUNCTION()
	void CheckArrived(AItem* ArrivedItem);

	UFUNCTION()
	void SetDeliveryRobot(AItem* TargetItem);

	UFUNCTION(BlueprintCallable)
	void DestoryDeliveryRobot(ADeliveryRobot* NewRobot);

	UFUNCTION(BlueprintCallable)
	void SetProductRobot(UProductRecipeDataAsset* ProductRecipe);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<AItemProductionCell*> ProductionCellList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	AItemManagerActor* ItemManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector SpawnItemPlusLoc = {0,0,0};

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnDeliveryRobotUpdate OnDeliveryRobotUpdate;

};