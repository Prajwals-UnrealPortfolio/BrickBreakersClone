// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBCGameMode.generated.h"

class ABBCPaddle;
class ABBCPlayerController;
class ABBCCamera;
/**
 * 
 */
UCLASS()
class BRICKBREAKERSCLONE_API ABBCGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void StartPlay() override;

private:

	UPROPERTY()
	ABBCCamera* BBCCamera;
	UPROPERTY()
	ABBCPlayerController* BBCPlayerController;
	UPROPERTY()
	ABBCPaddle* BBCPaddle;
	
};
