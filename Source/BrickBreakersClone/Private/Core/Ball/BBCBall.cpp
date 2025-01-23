// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Ball/BBCBall.h"

#include "Core/Paddle/BBCPaddle.h"

/**
 * @brief Constructor for the ABBCBall class, initializing a ball for a brick breaker game.
 *
 * Sets up the ball's physical properties, mesh, and initial state:
 * - Enables actor tick
 * - Creates a sphere mesh from the StarterContent
 * - Configures physics constraints to allow movement on X and Y axes
 * - Disables gravity and shadow casting
 * - Locks rotational movement
 * - Enables physics simulation and collision detection
 *
 * @param ObjectInitializer Reference to object initialization parameters
 *
 * @note Initializes ball with zero direction and velocity
 * @note Calls ResetBall() to set initial positioning
 */
ABBCBall::ABBCBall(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                  Direction(0.f,0.f,0.f),
                                                                  Velocity(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetActorEnableCollision(true);
	
	const ConstructorHelpers::FObjectFinder<UStaticMesh> BallRef(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BallMesh"));
	Mesh->SetStaticMesh(BallRef.Object);
	SetRootComponent(Mesh);
	Mesh->SetEnableGravity(false);
	Mesh->SetConstraintMode(EDOFMode::SixDOF);
	Mesh->GetBodyInstance()->bLockXRotation = true;
	Mesh->GetBodyInstance()->bLockYRotation = true;
	Mesh->GetBodyInstance()->bLockZRotation = true;
	Mesh->GetBodyInstance()->bLockXTranslation = false;
	Mesh->GetBodyInstance()->bLockYTranslation = false;
	Mesh->GetBodyInstance()->bLockZTranslation = true;
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetLinearDamping( 0.0f );
	Mesh->SetSimulatePhysics( true );
	Mesh->SetCastShadow(false);
	ResetBall();
}

/**
 * @brief Initializes the ball when the game starts or when the actor is spawned.
 *
 * This method is called automatically by the game engine when the ball is first created
 * or when the game begins. It calls the parent class's BeginPlay method and then
 * immediately resets the ball to its initial state using the ResetBall() method.
 *
 * @note Overrides the base class implementation to add custom initialization logic.
 */
void ABBCBall::BeginPlay()
{
	Super::BeginPlay();

	ResetBall();
}

/**
 * @brief Updates the ball's position every frame based on its current direction and velocity.
 *
 * This method is called automatically by the game engine for each frame. It moves the ball
 * by calculating a new translation based on the ball's direction, velocity, and the time
 * elapsed since the last frame (DeltaTime).
 *
 * @param DeltaTime The time elapsed since the last frame, used to calculate smooth movement.
 *
 * @note Calls the parent class's Tick method to ensure standard actor update behavior.
 * @note The ball's movement is frame-rate independent due to the use of DeltaTime.
 */
void ABBCBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FTransform Transform = GetActorTransform();
	Transform.AddToTranslation(Direction * Velocity * DeltaTime);
	SetActorTransform(Transform);
}

/**
 * @brief Handles collision events for the ball, updating its direction and responding to specific collision types.
 *
 * This method is called when the ball collides with another component. It performs the following actions:
 * - Mirrors the ball's direction based on the collision normal
 * - Resets the ball if it hits an "UnSafeBound" component
 * - Adjusts the ball's horizontal direction if it collides with a paddle, incorporating the paddle's velocity
 *
 * @param MyComp The primitive component of this ball that was involved in the collision
 * @param Other The actor that was hit
 * @param OtherComp The primitive component of the other actor that was hit
 * @param bSelfMoved Indicates whether this actor moved during the collision
 * @param HitLocation The world space location of the collision
 * @param HitNormal The normal vector of the collision surface
 * @param NormalImpulse The impulse applied during the collision
 * @param Hit Detailed information about the collision
 *
 * @note If the collision is with a paddle, the ball's X direction is modified based on the paddle's velocity
 * @note The method returns early if the paddle cannot be cast correctly
 */
void ABBCBall::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Direction = Direction.MirrorByVector(HitNormal).GetSafeNormal();
	
	if(OtherComp->ComponentHasTag("UnSafeBound"))
	{
		ResetBall();
	}
	
	if (Other->ActorHasTag("Paddle"))
	{
		ABBCPaddle* Paddle = Cast<ABBCPaddle>(Other);
		if(Paddle == nullptr)
		{
			return;
		}
		const float PaddleInfluence = FMath::Clamp(Paddle->GetPaddleVelocity() / Velocity,-0.75f, 0.75f);
		Direction.X += PaddleInfluence;
		Direction = Direction.GetSafeNormal();
	}
}

/**
 * @brief Initiates the ball's movement by setting its initial direction and velocity.
 *
 * Sets the ball's primary direction downward (negative Y-axis) and adds a random horizontal
 * component to create a more dynamic trajectory. The ball's velocity is set to a constant
 * speed of 300 units.
 *
 * @note The random X-axis component ensures the ball does not always move straight down,
 * adding unpredictability to its initial path.
 */
void ABBCBall::StartMoving()
{
	Direction = FVector( 0.f, -1.f, 0.f );
	Direction.X = FMath::RandRange(-1.f,1.f);
	Velocity = 300.0f;
}

/**
 * @brief Resets the ball to its initial state and position.
 *
 * This method performs the following actions:
 * - Sets the ball's location to a predefined fixed point (0, 370, 0)
 * - Scales the ball down to 30% of its original size
 * - Clears the ball's current direction
 * - Resets the ball's velocity to zero
 *
 * @note Typically used to return the ball to its starting configuration, such as after losing a life or at the beginning of a game.
 */
void ABBCBall::ResetBall()
{
	SetActorLocation(FVector(0.f,370.f,0.f));
	SetActorScale3D(FVector(0.3f,0.3f,0.3f));
	Direction = FVector::Zero();
	Velocity = 0.f;
}


