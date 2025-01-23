// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBCGameState.generated.h"

class ABBCBall;
class ABBCPlayerController;
/**
 * Current score
 * Number of remaining bricks
 * Game status (in progress, paused, game over)
 * Current level information
 */
UCLASS()
class BRICKBREAKERSCLONE_API ABBCGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	void SetPlayerControllerAndBall(ABBCPlayerController* BBCController, ABBCBall* Ball);

	void TryStartBall();

private:

	UPROPERTY()
	TWeakObjectPtr<ABBCPlayerController> BBCPlayerController;
	UPROPERTY()
	TWeakObjectPtr<ABBCBall> BBCBall;
	
};

