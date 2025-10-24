// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset/ProductRecipeStruct.h"
#include "ProductRecipeDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FACTORYPROJECT_API UProductRecipeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ProductName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FProductRecipeStruct> RecipeData;

};
