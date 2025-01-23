// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BBCBall.generated.h"

UCLASS()
class BRICKBREAKERSCLONE_API ABBCBall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABBCBall(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void StartMoving();
	void ResetBall();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	FVector Direction;

	UPROPERTY(VisibleAnywhere)
	float Velocity;
};
