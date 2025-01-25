// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BrickLevelData.generated.h"

UENUM()
enum EBRICK_TYPE
{
	EMPTY,
	NORMAL,
	CHAINED,
	FIRE,
	MULTIPLE_BALL,
	SIZE_INCREASE,
	SIZE_DECREASE,
	COUNT
};

USTRUCT(BlueprintType)
struct FBrickInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick Data")
	int32 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick Data")
	int32 Column;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick Data")
	TEnumAsByte<EBRICK_TYPE> BrickState;
};

USTRUCT(BlueprintType)
struct FBrickColor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick Data")
	TEnumAsByte<EBRICK_TYPE> BrickType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick Data")
	FLinearColor BrickColor;
};

/**
 * 
 */
UCLASS(BlueprintType)
class BRICKBREAKERSCLONE_API UBrickLevelData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data")
	TArray<FBrickInfo> Bricks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data")
	FVector2D GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data")
	float CellSize;
};
