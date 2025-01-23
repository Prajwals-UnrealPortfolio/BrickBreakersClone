// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BBCGameState.h"
#include "Core/Ball/BBCBall.h"
#include "PlayerController/BBCPlayerController.h"

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
