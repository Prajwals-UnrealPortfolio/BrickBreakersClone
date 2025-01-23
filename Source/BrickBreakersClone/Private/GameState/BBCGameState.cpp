// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BBCGameState.h"
#include "Core/Ball/BBCBall.h"
#include "PlayerController/BBCPlayerController.h"

ABBCGameState::ABBCGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABBCGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ABBCGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

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
