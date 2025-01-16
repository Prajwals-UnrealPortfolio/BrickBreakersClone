// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BBCGameMode.h"
#include "Cameras/BBCCamera.h"
#include "PlayerController/BBCPlayerController.h"

void ABBCGameMode::StartPlay()
{
	Super::StartPlay();
	UWorld* World = GetWorld();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	BBCCamera = World->SpawnActor<ABBCCamera>(ABBCCamera::StaticClass());
	if((!ensure(BBCCamera)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Camera"));
		return;
	}

	BBCPlayerController = Cast<ABBCPlayerController>(World->GetFirstPlayerController());
	if((!ensure(BBCPlayerController)))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Player Controller"));
		return;
	}

	BBCPlayerController->SetViewTarget(BBCCamera);
}
