// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BBCGameState.h"
#include "Core/Ball/BBCBall.h"
#include "PlayerController/BBCPlayerController.h"

/**
 * @brief Constructor for the ABBCGameState class.
 *
 * Initializes the game state and enables per-frame tick updates.
 * By setting PrimaryActorTick.bCanEverTick to true, this allows the Tick() method
 * to be called every frame, enabling continuous game state monitoring and updates.
 */
ABBCGameState::ABBCGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * @brief Initializes the game state when play begins.
 *
 * Calls the parent class's BeginPlay method to ensure proper initialization
 * of the base game state. This method can be extended to add custom
 * initialization logic specific to the Brick Breakers Clone game.
 */
void ABBCGameState::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * @brief Updates the game state every frame and handles space bar input for ball movement.
 *
 * This method is called every frame and checks if the space bar key was just released.
 * If the space bar is released and both the player controller and ball are valid,
 * it triggers the ball to start moving.
 *
 * @param DeltaSeconds The time elapsed since the last frame.
 *
 * @note Overrides the parent class's Tick method to add custom game-specific logic.
 * @note Requires valid BBCPlayerController and BBCBall pointers to function.
 */
void ABBCGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(BBCPlayerController != nullptr)
	{
		if ( BBCPlayerController->WasInputKeyJustReleased( EKeys::SpaceBar ) )
		{
			if(BBCBall != nullptr)
			{
				BBCBall->StartMoving();
			}
		}
	}
	
}

/**
 * @brief Sets the player controller and ball for the game state.
 *
 * This method assigns the provided player controller and ball objects to the game state,
 * allowing the game state to track and interact with these key game components.
 *
 * @param BBCController Pointer to the player controller to be set in the game state.
 * @param Ball Pointer to the ball object to be set in the game state.
 */
void ABBCGameState::SetPlayerControllerAndBall(ABBCPlayerController* BBCController, ABBCBall* Ball)
{
	BBCPlayerController = BBCController;
	BBCBall = Ball;
}
