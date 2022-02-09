// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/BasePawn.h"
#include "ChHorse.generated.h"

/**
 * 
 */
UCLASS()
class SCHESS_API AChHorse : public ABasePawn
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	virtual void ConfigurePawn() override;
};
