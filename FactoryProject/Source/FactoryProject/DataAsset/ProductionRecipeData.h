// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset/ItemBasicDataAsset.h"
#include "ProductionRecipeData.generated.h"


USTRUCT(BlueprintType)
struct FProductionProgressStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UItemBasicDataAsset* ItemData;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemProgress; // 0 : 진행 이전, 1 : 진행 중, 2 : 진행 완료

};

UCLASS(BlueprintType, Blueprintable)
class FACTORYPROJECT_API UProductionRecipeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
};
