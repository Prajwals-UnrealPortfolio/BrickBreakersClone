	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBCPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class BRICKBREAKERSCLONE_API ABBCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ABBCPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:

	void OnStartBall();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = Input ,meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = Input ,meta=(AllowPrivateAccess = "true"))
	UInputAction* StartInputAction;
};
