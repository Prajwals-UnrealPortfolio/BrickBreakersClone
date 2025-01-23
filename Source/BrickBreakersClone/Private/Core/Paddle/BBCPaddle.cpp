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
ABBCPaddle::ABBCPaddle():
MovementSpeed(350.f),
MaxBoundaryLength(0.f)
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
	
	if(Controller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is Invalid"));
		return;
	}
	
	BBCPlayerController = Cast<ABBCPlayerController>(Controller);
	if(BBCPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BBCPlayerController is Invalid"));
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(BBCPlayerController->GetLocalPlayer());
	if(Subsystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Subsystem is Invalid"));
		return;
	}

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PlayerInputMappingContext.Get(), 0);
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
	if(EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent is Invalid"));
		return;
	}
	EnhancedInputComponent->BindAction(MoveInputAction.Get(), ETriggerEvent::Triggered, this, &ABBCPaddle::MoveLeftOrRight);
}

void ABBCPaddle::SetMaxBoundaryLength(float Value)
{
	MaxBoundaryLength = Value;
}

void ABBCPaddle::MoveLeftOrRight(const FInputActionValue& Value)
{
	InputDirection = Value.Get<float>();
	InputDirection = FMath::Clamp(InputDirection, -1.f,1.f);
	Velocity = InputDirection * MovementSpeed;
	FVector Location = GetActorLocation();
	Location += FVector::ForwardVector * Velocity * GetWorld()->GetDeltaSeconds();
	Location.X = FMath::Clamp(Location.X, -MaxBoundaryLength, MaxBoundaryLength);
	SetActorLocation(Location);
}

