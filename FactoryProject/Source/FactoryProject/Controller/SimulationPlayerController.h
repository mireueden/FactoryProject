// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"

#include "SimulationPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYPROJECT_API ASimulationPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASimulationPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
	TArray<ACameraActor*> ObservationCameras;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
	float BlendTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
	int32 CurrentCameraIndex = 0;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CameraSetting();

	UFUNCTION(BlueprintCallable)
	void SwitchToCamera(int32 Index);

	UFUNCTION(BlueprintCallable)
	void NextCamera();
};
