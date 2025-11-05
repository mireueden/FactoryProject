// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SimulationPlayerController.h"

#include "Kismet/GameplayStatics.h" 

ASimulationPlayerController::ASimulationPlayerController()
{
}

void ASimulationPlayerController::BeginPlay()
{
    Super::BeginPlay();



}

void ASimulationPlayerController::CameraSetting()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

    ObservationCameras.Empty();

    for (AActor* Actor : FoundActors)
    {
        ACameraActor* Cam = Cast<ACameraActor>(Actor);
        if (Cam)
        {
            ObservationCameras.Add(Cam);
        }
    }

    if (ObservationCameras.Num() > 0)
        SetViewTargetWithBlend(ObservationCameras[0], 0.0f);
}

void ASimulationPlayerController::SwitchToCamera(int32 Index)
{
    if (ObservationCameras.IsValidIndex(Index))
        SetViewTargetWithBlend(ObservationCameras[Index], BlendTime);
}

void ASimulationPlayerController::NextCamera()
{
    CurrentCameraIndex = (CurrentCameraIndex + 1) % ObservationCameras.Num();
    SwitchToCamera(CurrentCameraIndex);
}
