// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BBCGameMode.h"

#include "Camera/CameraComponent.h"
#include "Cameras/BBCCamera.h"
#include "Core/Paddle/BBCPaddle.h"
#include "PlayerController/BBCPlayerController.h"

void ABBCGameMode::StartPlay()
{
	Super::StartPlay();
	UWorld* World = GetWorld();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	BBCCamera = World->SpawnActor<ABBCCamera>(ABBCCamera::StaticClass());
	if((!ensure(BBCCamera)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Camera. Ensure CameraClass is set in Blueprint."));
		return;
	}

	BBCPlayerController = Cast<ABBCPlayerController>(World->GetFirstPlayerController());
	if((!ensure(BBCPlayerController)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Player Controller. Ensure game mode is properly set in project settings."));
		return;
	}

	BBCPlayerController->SetViewTarget(BBCCamera);

	BBCPaddle = Cast<ABBCPaddle>(BBCPlayerController->GetPawn());

	if((!ensure(BBCPaddle)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get BBCPaddle. Ensure game mode is properly set in project settings."));
		return;
	}
	float MaxBoundaryLength = BBCCamera->GetCameraComponent()->OrthoWidth;
	MaxBoundaryLength/=2;
	MaxBoundaryLength-=48.f;
	BBCPaddle->SetMaxBoundaryLength(MaxBoundaryLength);
}
