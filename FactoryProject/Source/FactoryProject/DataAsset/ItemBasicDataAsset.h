// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemBasicDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UItemBasicDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 ItemIndex;

	UPROPERTY(BlueprintReadWrite)
	FString ItemName;
};
