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
struct FPawnInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<PawnTypes> PawnType;

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* PawnMesh;

	UPROPERTY(BlueprintReadWrite)
	UMaterial* PawnMeshWhiteMaterial;

	UPROPERTY(BlueprintReadWrite)
	UMaterial* PawnMeshBlackMaterial;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<PawnColorType> PawnColor;
};
/**
 * 
 */
class SCHESS_API Globals
{
public:
	Globals();
	~Globals();
};
