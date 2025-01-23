// Fill out your copyright notice in the Description page of Project Settings.


#include "Cameras/BBCCamera.h"
#include "Camera/CameraComponent.h"

ABBCCamera::ABBCCamera() :
FieldOfView(120.f),
SpawnLocation(0.f,0.f,500.f),
SpawnRotation(-90.f,0.f,-90.f)
{
	BBCCameraComponent = GetCameraComponent();
	SetActorRelativeLocation(SpawnLocation);
	SetActorRelativeRotation(SpawnRotation);
	SetActorRelativeScale3D(FVector::OneVector);
	BBCCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	BBCCameraComponent->SetFieldOfView(FieldOfView);
}
