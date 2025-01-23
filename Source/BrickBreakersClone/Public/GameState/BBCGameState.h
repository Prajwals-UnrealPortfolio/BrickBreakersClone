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

	ABBCGameState();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void SetPlayerControllerAndBall(ABBCPlayerController* BBCController, ABBCBall* Ball);

private:

	UPROPERTY()
	ABBCPlayerController* BBCPlayerController;
	UPROPERTY()
	ABBCBall* BBCBall;
	
};
