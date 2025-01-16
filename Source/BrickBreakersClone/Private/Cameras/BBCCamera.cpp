// Fill out your copyright notice in the Description page of Project Settings.


#include "Cameras/BBCCamera.h"

ABBCCamera::ABBCCamera()
{
	BBCCameraComponent = GetCameraComponent();
	SetActorRelativeLocation(FVector(0.f,0.f,500.f));
	SetActorRelativeRotation(FRotator(-90.f, 0.f,0.f));
	SetActorRelativeScale3D(FVector::OneVector);
}