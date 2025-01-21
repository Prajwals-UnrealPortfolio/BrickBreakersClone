// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Paddle/BBCPaddle.h"
#include "Cameras/BBCCamera.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerController/BBCPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameMode/BBCGameMode.h"

// Sets default values
ABBCPaddle::ABBCPaddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PaddleSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PaddleSceneComponent"));
	SetRootComponent(PaddleSceneComponent);
	PaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadddleMesh"));
	PaddleMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABBCPaddle::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(FVector(0.f,400.f,0.f));
	SetActorRotation(FRotator::ZeroRotator);
	SetActorScale3D(FVector(2.f,1.f,1.f));
	
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
	InputDirection = Value.Get<float>();
	Velocity = InputDirection * 350.f;
	InputDirection = FMath::Clamp(InputDirection, -1.f,1.f);
	FVector Location = GetActorLocation();
	Location += FVector::ForwardVector * 350.f * InputDirection * GetWorld()->GetDeltaSeconds();
	Location.X = FMath::Clamp(Location.X, -720.f, 720.f);
	SetActorLocation(Location);
}

