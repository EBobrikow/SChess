// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Actors/BoardCell.h"
#include "Actors/Pawns/BasePawn.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Globals.h"
#include "SChessGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SCHESS_API ASChessGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* BoardCellBlackMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* BoardCellWhiteMesh;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABasePawn> ChessPawnClass;

protected:

	virtual void BeginPlay() override;

private: 

	UPROPERTY()
	TArray<ABoardCell*> DeskArray;

	UFUNCTION()
	void CreateDesk();
	
};
