// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Paddle/BBCPaddle.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerController/BBCPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "InputActionValue.h"

/**
 * @brief Constructor for the ABBCPaddle class, initializing paddle properties and components.
 *
 * Sets up the paddle's default movement speed, creates scene and mesh components,
 * and configures collision settings for the paddle mesh.
 *
 * @details
 * - Enables actor tick for per-frame updates
 * - Creates a root scene component and a static mesh component
 * - Configures paddle mesh collision:
 *   - Enables query and physics collision
 *   - Sets collision object type to Pawn
 *   - Sets collision response to overlap with world static objects
 *   - Disables physics simulation
 *
 * @note Default movement speed is set to 350 units per second
 * @note Maximum boundary length is initially set to 0
 */
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

	PaddleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PaddleMesh->SetCollisionObjectType(ECC_Pawn);
	PaddleMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	PaddleMesh->SetSimulatePhysics(false);
}

/**
 * @brief Initializes the paddle actor when the game starts or when spawned.
 *
 * This method sets up the initial position, rotation, and scale of the paddle, 
 * adds a "Paddle" tag, and configures input mapping for the player controller.
 *
 * @details The method performs the following key actions:
 * - Sets the actor's location to (0, 400, 0)
 * - Resets the actor's rotation to zero
 * - Scales the actor to (2, 1, 1)
 * - Adds a "Paddle" tag to the actor
 * - Validates the controller and player controller
 * - Sets up enhanced input mapping context
 *
 * @note Logs error messages if:
 * - Controller is invalid
 * - Player controller cast fails
 * - Enhanced input subsystem is unavailable
 *
 * @throws None Handles potential errors by logging and early returning
 */
void ABBCPaddle::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(FVector(0.f,400.f,0.f));
	SetActorRotation(FRotator::ZeroRotator);
	SetActorScale3D(FVector(2.f,1.f,1.f));

	this->Tags.Add("Paddle");
	
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
	Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
}

/**
 * @brief Updates the paddle's state every frame.
 *
 * @param DeltaTime The time elapsed since the last frame.
 *
 * @details Currently calls the parent class's Tick method without additional logic.
 * This method is called automatically by the game engine for each frame.
 *
 * @note Override of the base class Tick method to maintain the default frame update behavior.
 */
void ABBCPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * @brief Sets up player input component for paddle movement.
 *
 * Configures the enhanced input component to bind the paddle's movement action.
 * Attempts to cast the input component to UEnhancedInputComponent and logs an error
 * if the cast fails. Binds the MoveInputAction to the MoveLeftOrRight method
 * when the input is triggered.
 *
 * @param PlayerInputComponent The input component to be configured for player input.
 * @note Overrides the parent class's SetupPlayerInputComponent method.
 * @warning Requires a valid UEnhancedInputComponent for successful input binding.
 */
void ABBCPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent is Invalid"));
		return;
	}
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ABBCPaddle::MoveLeftOrRight);
}

/**
 * @brief Sets the maximum boundary length for the paddle's movement.
 * 
 * @param Value The maximum distance the paddle can move from its initial position.
 * 
 * @note This method allows dynamic adjustment of the paddle's movement constraints.
 */
void ABBCPaddle::SetMaxBoundaryLength(float Value)
{
	MaxBoundaryLength = Value;
}

void ABBCPaddle::MoveLeftOrRight(const FInputActionValue& Value)
{
	InputDirection = Value.Get<float>();
	InputDirection = FMath::Clamp(InputDirection, -1.f,1.f);
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	const FVector TargetMovement = FVector::ForwardVector * InputDirection * MovementSpeed * DeltaTime;
	const FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, CurrentLocation + TargetMovement, DeltaTime,100.0f);
	NewLocation.X = FMath::Clamp(NewLocation.X, -MaxBoundaryLength, MaxBoundaryLength);
	SetActorLocation(NewLocation);
}

