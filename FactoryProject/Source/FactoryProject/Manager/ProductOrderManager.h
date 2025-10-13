// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DataAsset/ItemManagerActor.h"
#include "Object/ItemProductionCell.h"

#include "ProductOrderManager.generated.h"

UCLASS()
class FACTORYPROJECT_API AProductOrderManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProductOrderManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	AItemManagerActor* ItemManager; // 재료들의 개수들을 모두 가지고 있음

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<AItemProductionCell*> ProductionCellList;

	UFUNCTION()
	void ProductionCellSetting();

	//UFUNCTION()
	


};
