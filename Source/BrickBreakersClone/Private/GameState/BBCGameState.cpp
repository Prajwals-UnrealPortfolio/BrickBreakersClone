// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BBCGameState.h"
#include "Core/Ball/BBCBall.h"
#include "PlayerController/BBCPlayerController.h"

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

void ABBCGameState::TryStartBall()
{
	if(BBCBall != nullptr)
	{
		BBCBall->StartMoving();
	}
}


