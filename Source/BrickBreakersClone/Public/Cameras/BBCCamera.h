// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "BBCCamera.generated.h"

/**
 * 
 */
UCLASS()
class BRICKBREAKERSCLONE_API ABBCCamera : public ACameraActor
{
	GENERATED_BODY()

public:

	ABBCCamera();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* BBCCameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (MakeEditWidget = true))
	FVector SpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FRotator SpawnRotation;
};
