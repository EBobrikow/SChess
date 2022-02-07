// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/BasePawn.h"
#include "Globals.h"
#include "ChPawn.generated.h"

/**
 * 
 */
UCLASS()
class SCHESS_API AChPawn : public ABasePawn
{
	GENERATED_BODY()

public:

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<PawnTypes> PawnType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* PawnMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterial* PawnMeshWhiteMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterial* PawnMeshBlackMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<PawnColorType> PawnColor;*/

	

protected:

	virtual void BeginPlay() override;
	
	
	

public:
	AChPawn();

};
