// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BBCGameMode.h"

#include "Camera/CameraComponent.h"
#include "GameState/BBCGameState.h"
#include "Cameras/BBCCamera.h"
#include "Core/Paddle/BBCPaddle.h"
#include "Core/Ball/BBCBall.h"
#include "PlayerController/BBCPlayerController.h"

/**
 * @brief Initializes the game state and spawns essential game actors.
 *
 * This method is responsible for setting up the game environment by:
 * - Spawning the game camera
 * - Setting up the player controller
 * - Configuring the paddle's movement boundaries
 * - Spawning and resetting the game ball
 * - Updating the game state with player and ball references
 *
 * @note Performs multiple error checks to ensure critical components are properly initialized
 * @note Logs error messages if any critical initialization steps fail
 *
 * @throws No exceptions are thrown, but error conditions are logged
 *
 * @pre Requires a valid game world and properly configured game mode settings
 * @post Game camera, player controller, paddle, and ball are initialized
 */
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
