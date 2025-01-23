// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Ball/BBCBall.h"

#include "Core/Paddle/BBCPaddle.h"

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

// Called when the game starts or when spawned
void ABBCBall::BeginPlay()
{
	Super::BeginPlay();

	ResetBall();
}

// Called every frame
void ABBCBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FTransform Transform = GetActorTransform();
	Transform.AddToTranslation(Direction * Velocity * DeltaTime);
	SetActorTransform(Transform);
}

void ABBCBall::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Direction = Direction.MirrorByVector(HitNormal);
	
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
		Direction.X += (Paddle->GetPaddleVelocity() / Velocity);
	}
}

void ABBCBall::StartMoving()
{
	Direction = FVector( 0.f, -1.f, 0.f );
	Direction.X = FMath::RandRange(-1.f,1.f);
	Velocity = 300.0f;
}

void ABBCBall::ResetBall()
{
	SetActorLocation(FVector(0.f,370.f,0.f));
	SetActorScale3D(FVector(0.3f,0.3f,0.3f));
	Direction = FVector::Zero();
	Velocity = 0.f;
}


