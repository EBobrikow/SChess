// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Globals.generated.h"


UENUM(BlueprintType)
enum class PawnTypes : uint8 
{
	None,
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

USTRUCT(BlueprintType)
struct FPawnMovementInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
		int32 FirstCellIndexX;

	UPROPERTY(BlueprintReadWrite)
		int32 FirstCellIndexY;

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<PawnTypes> FirstCellPawnType;

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<PawnColorType> FirstCellPawnColor;


	UPROPERTY(BlueprintReadWrite)
		int32 SecondCellIndexX;

	UPROPERTY(BlueprintReadWrite)
		int32 SecondCellIndexY;

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<PawnTypes> SecondCellPawnType;

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<PawnColorType> SecondCellPawnColor;

	FPawnMovementInfo();

	FPawnMovementInfo(int32 FCindX, int32 FCindY, TEnumAsByte<PawnTypes> FCPawnType, TEnumAsByte<PawnColorType> FCPawnColor,
					  int32 SCindX, int32 SCindY, TEnumAsByte<PawnTypes> SCPawnType, TEnumAsByte<PawnColorType> SCPawnColor);

};


UENUM(BlueprintType)
enum class MovementDirections : uint8
{
	Forward,
	Forward_Left,
	Left,
	Back_Left,
	Back,
	Back_Right,
	Right,
	Forward_Right,
	T_Forward_Right,
	T_Forward_Left,
	T_Left_Right,
	T_Left_Left,
	T_Back_Right,
	T_Back_Left,
	T_Right_Right,
	T_Right_Left,
};

UENUM(BlueprintType)
enum class MovementRange : uint8
{
	OneStep,
	Infinite,
};


class SCHESS_API Globals
{
public:
	Globals();
	~Globals();
};
