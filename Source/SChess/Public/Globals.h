// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Globals.generated.h"


UENUM(BlueprintType)
enum class PawnTypes : uint8 
{
	Pawn,
	Bishop,
	Horse,
	Tour,
	King,
	Queen,
};

UENUM(BlueprintType)
enum class PawnColorType : uint8
{
	None,
	Black,
	White,
};


USTRUCT(BlueprintType)
struct FPawnBaseLocationInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int32 IndexX;

	UPROPERTY(BlueprintReadWrite)
	int32 IndexY;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<PawnColorType> PawnSideColor;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<PawnTypes> PawnType;


	FPawnBaseLocationInfo();

	FPawnBaseLocationInfo(int32 X, int32 Y, TEnumAsByte<PawnColorType> SideColor, TEnumAsByte<PawnTypes> Type);
};



class SCHESS_API Globals
{
public:
	Globals();
	~Globals();
};
