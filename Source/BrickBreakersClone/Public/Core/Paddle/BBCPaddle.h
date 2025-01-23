// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BBCPaddle.generated.h"

class ABBCPlayerController;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class BRICKBREAKERSCLONE_API ABBCPaddle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABBCPaddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetMaxBoundaryLength(float Value);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = Input ,meta=(AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext> PlayerInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = Input ,meta=(AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* PaddleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Scene, meta=(AllowPrivateAccess = "true"))
	USceneComponent* PaddleSceneComponent;

	UPROPERTY()
	ABBCPlayerController* BBCPlayerController;

	UPROPERTY()
	float InputDirection;
	UPROPERTY()
	float Velocity;
	UPROPERTY()
	float MovementSpeed;
	UPROPERTY()
	float MaxBoundaryLength;

private:
	
	void MoveLeftOrRight(const FInputActionValue& Value);

};
