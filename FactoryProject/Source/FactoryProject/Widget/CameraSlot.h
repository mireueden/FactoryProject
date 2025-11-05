// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"


#include "CameraSlot.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API UCameraSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, ExposeOnSpawn = "true"))
	class UTextBlock* CameraName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SwitchCameraBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 CameraIndex = -1;
};
