// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAsset/ItemBasicDataAsset.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None      UMETA(DisplayName = "None"),
	Move      UMETA(DisplayName = "Move"),
	Stop      UMETA(DisplayName = "Stop")
};

UCLASS()
class FACTORYPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowedClasses = "ItemBasicDataAsset"))
	UItemBasicDataAsset* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemState ItemState;

	UPROPERTY()
	float SplineProgress = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

};
