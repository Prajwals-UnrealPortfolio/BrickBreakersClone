// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Paddle/BBCPaddle.h"

#include "EnhancedInputSubsystems.h"
#include "PlayerController/BBCPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "InputActionValue.h"

// Sets default values
ABBCPaddle::ABBCPaddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABBCPaddle::BeginPlay()
{
	Super::BeginPlay();

	checkf(Controller, TEXT("Controller is Invalid"));
	
	BBCPlayerController = Cast<ABBCPlayerController>(Controller);
	checkf(BBCPlayerController, TEXT("BBCPlayerController is Invalid"));
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(BBCPlayerController->GetLocalPlayer());
	checkf(BBCPlayerController, TEXT("Subsystem is Invalid"));

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PlayerInputMappingContext.LoadSynchronous(), 0);
}

// Called every frame
void ABBCPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABBCPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	checkf(EnhancedInputComponent, TEXT("EnhancedInputComponent is invalid"));
	EnhancedInputComponent->BindAction(MoveInputAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ABBCPaddle::MoveLeftOrRight);
}

void ABBCPaddle::MoveLeftOrRight(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Movement Axis : %f"), Value.Get<float>());
}

