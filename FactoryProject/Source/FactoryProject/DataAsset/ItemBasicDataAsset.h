// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemBasicDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FItemMeshAttachData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UStaticMesh> ItemMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PaintSlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName TargetSocket;
};

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class FACTORYPROJECT_API UItemBasicDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UMaterialInterface* ItemMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItemMeshAttachData> ItemMeshes;


};
