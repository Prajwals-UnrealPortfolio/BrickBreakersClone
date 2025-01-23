// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BBCGameMode.h"

#include "Camera/CameraComponent.h"
#include "GameState/BBCGameState.h"
#include "Cameras/BBCCamera.h"
#include "Core/Paddle/BBCPaddle.h"
#include "Core/Ball/BBCBall.h"
#include "PlayerController/BBCPlayerController.h"

void ABBCGameMode::StartPlay()
{
	Super::StartPlay();
	UWorld* World = GetWorld();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	BBCCamera = World->SpawnActor<ABBCCamera>(ABBCCamera::StaticClass(), SpawnParameters);
	if((!ensure(BBCCamera)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Camera. Ensure CameraClass is set in Blueprint."));
		return;
	}

	BBCPlayerController = Cast<ABBCPlayerController>(World->GetFirstPlayerController());
	if((!ensure(BBCPlayerController)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Player Controller. "));
		return;
	}

	BBCPlayerController->SetViewTarget(BBCCamera);

	BBCPaddle = Cast<ABBCPaddle>(BBCPlayerController->GetPawn());

	if((!ensure(BBCPaddle)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get BBCPaddle. "));
		return;
	}
	float MaxBoundaryLength = BBCCamera->GetCameraComponent()->OrthoWidth;
	MaxBoundaryLength/=2;
	MaxBoundaryLength-=98.f;
	BBCPaddle->SetMaxBoundaryLength(MaxBoundaryLength);

	BBCBall = World->SpawnActor<ABBCBall>(ABBCBall::StaticClass(), SpawnParameters);
	if((!ensure(BBCBall)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Ball. "));
		return;
	}
	BBCBall->ResetBall();

	BBCGameState = GetGameState<ABBCGameState>();
	if((!ensure(BBCGameState)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get BBCGameState. "));
		return;
	}
	BBCGameState->SetPlayerControllerAndBall(BBCPlayerController, BBCBall);
}
