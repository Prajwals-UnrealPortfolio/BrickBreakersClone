// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BBCPlayerController.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameState/BBCGameState.h"

ABBCPlayerController::ABBCPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMapContRef(TEXT("InputMappingContext'/Game/Inputs/IMC_Player.IMC_Player'"));
	ConstructorHelpers::FObjectFinder<UInputAction> InputActionRef(TEXT("InputAction'/Game/Inputs/InputActions/IA_Start.IA_Start'"));

	PlayerInputMappingContext = InputMapContRef.Object;
	StartInputAction = InputActionRef.Object;
}

void ABBCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Subsystem is Invalid"));
		return;
	}
	Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
}

void ABBCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Enhanced Input Component is Invalid"));
		return;
	}

	EnhancedInputComponent->BindAction(StartInputAction, ETriggerEvent::Triggered, this, &ABBCPlayerController::OnStartBall);
}

void ABBCPlayerController::OnStartBall()
{
	if(ABBCGameState* GameState = GetWorld()->GetGameState<ABBCGameState>())
	{
		GameState->TryStartBall();
	}
}
