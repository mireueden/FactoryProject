// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Object/ItemProductionCell.h"
#include "DataAsset/ProductRecipeDataAsset.h"

#include "DeliveryRobot.generated.h"


UENUM(BlueprintType)
enum class ERobotState : uint8
{
	None,
	Moving,
	Waiting,
	Arrived,
	Working,
	Delivery,
	Returning,
	Error,
};

UENUM(BlueprintType)
enum class ERobotProcess : uint8
{
	None,
	Delivery,
	ProductProcess,
};


USTRUCT(BlueprintType)
struct FProgressOfItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product", meta = (AllowedClasses = "ItemBasicDataAsset"))
	UItemBasicDataAsset* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product", meta = (AllowedClasses = "ItemBasicDataAsset"))
	int32 ProgressValue = 0;

};

USTRUCT(BlueprintType)
struct FProgressOfRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ProductName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FProgressOfItem> RecipeData;
};

USTRUCT(BlueprintType)
struct FAttachedPartInfo
{
	GENERATED_BODY()

	UPROPERTY()
	UStaticMeshComponent* PartComponent;

	UPROPERTY()
	int32 PaintSlotIndex = 0;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	ERobotProcess CurrentProcess;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	AItem* TargetItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	AItemProductionCell* TargetCell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector TargetPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector ReturnPoint;

	UFUNCTION()
	void SetProcessState();
	
	UFUNCTION()
	void SetTargetCell();

	UFUNCTION()
	void SetTargetItem(AItem* Item);

	UFUNCTION()
	void SetRobotState();

	UFUNCTION()
	void AttachItem();

	UFUNCTION()
	void DetachItem();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	FProgressOfRecipe ProgressOfProductRecipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	UProductRecipeDataAsset* ProductRecipeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FAttachedPartInfo> AttachedParts;

	UFUNCTION()
	void ProductRecipeSetting(UProductRecipeDataAsset* ProductRecipe);

	UFUNCTION()
	void ArrivedCell();

	UFUNCTION()
	void ProcessCompletionInRobot();


};
