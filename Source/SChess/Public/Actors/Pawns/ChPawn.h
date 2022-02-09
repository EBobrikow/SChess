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

	

protected:

	virtual void BeginPlay() override;
	
	virtual void ConfigurePawn() override;
	

public:
	AChPawn();

};
