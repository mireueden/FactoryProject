// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Object/Item.h"

#include "ItemProductionCell.generated.h"

UCLASS()
class FACTORYPROJECT_API AItemProductionCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemProductionCell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UItemBasicDataAsset* ProductProcessData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	AItem* ProcessItem;

	UFUNCTION()
	void CraftingProduct();

};
