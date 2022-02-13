// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/BasePawn.h"
//#include "Globals.h"
//#include "Actors/BoardCell.h"
//#include "Kismet/GameplayStatics.h"
//#include "SChessGameModeBase.h"
#include "ChBishop.generated.h"

/**
 * 
 */
UCLASS()
class SCHESS_API AChBishop : public ABasePawn
{
	GENERATED_BODY()

public: 

	//virtual TArray<ABoardCell*> GetPossibleSteps() override;
	
protected:

	virtual void BeginPlay() override;

	virtual void ConfigurePawn() override;
};
