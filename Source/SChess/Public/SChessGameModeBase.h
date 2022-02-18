// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Actors/BoardCell.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Globals.h"
#include "Actors/PawnMovementLogger.h"
#include "SChessGameModeBase.generated.h"


#define TOTAL_FIGURES_NUM 32

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnMove, FPawnMovementInfo, MovementInfo);

/**
 * 
 */
UCLASS()
class SCHESS_API ASChessGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FOnPawnMove OnPawnMoveDelegate;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* BoardCellBlackMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* BoardCellWhiteMesh;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABasePawn> ChessPawnClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABasePawn> ChessBishopClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABasePawn> ChessHorseClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABasePawn> ChessTourClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABasePawn> ChessQueenClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABasePawn> ChessKingClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInstance* HighlightMaterial;

	UFUNCTION(BlueprintCallable)
	ABasePawn* GetPawnOnCellByIndex(int32 IndexX, int32 IndexY) const;

	UFUNCTION(BlueprintCallable)
	void SetPawnOnCellByIndex(ABasePawn* PawnToSet, int32 IndexX, int32 IndexY);

	UFUNCTION(BlueprintCallable)
	ABoardCell* GetCellByIndex(int32 X, int32 Y) const;

	UFUNCTION(BlueprintCallable)
	void UndoLastMove();


	UFUNCTION(BlueprintCallable)
	void AddDynaMatForPostProcessVolume();

	UFUNCTION(BlueprintCallable)
	void UpdatePostProcessMaterial(int32 X, int32 Y, int32 Z);

	UFUNCTION(BlueprintCallable)
	void StartMovePawn(ABoardCell* FromCell, ABoardCell* ToCell);

	UFUNCTION(BlueprintCallable)
	bool IsPawnExistOnCell(int32 X, int32 Y, TEnumAsByte<PawnColorType> PawnColor, bool& IsSameColor);

	UFUNCTION(BlueprintCallable)
	void UnHighlightAll();

	
	TArray<ABoardCell*> GetForbiddenCellsForKing( TEnumAsByte<PawnColorType> KingColor);

protected:

	virtual void BeginPlay() override;

private: 

	UPROPERTY()
	TArray<ABoardCell*> DeskArray;

	UPROPERTY()
	TArray<ABasePawn*> WhitePawns;

	UPROPERTY()
	TArray<ABasePawn*> BlackPawns;

	UFUNCTION()
	void CreateDesk();

	UFUNCTION()
	void SpawnChessPawn(ABoardCell* CellActorOnSpawn, TSubclassOf<ABasePawn> PawnClass, TEnumAsByte<PawnColorType> PawnColor);

	UFUNCTION()
	void InitStartupArragment();

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicHighlightMaterial = nullptr;

	UPROPERTY()
	APawnMovementLogger* MovementLogger = nullptr;

	UFUNCTION()
	TSubclassOf<ABasePawn> GetSubclassOfPawnType(TEnumAsByte<PawnTypes> Type);


	FPawnBaseLocationInfo InitialFiguresArrangment[TOTAL_FIGURES_NUM] = {
	FPawnBaseLocationInfo(1,1,PawnColorType::White, PawnTypes::Tour) ,
	FPawnBaseLocationInfo(2,1,PawnColorType::White, PawnTypes::Horse) ,
	FPawnBaseLocationInfo(3,1,PawnColorType::White, PawnTypes::Bishop) ,
	FPawnBaseLocationInfo(4,1,PawnColorType::White, PawnTypes::King) ,
	FPawnBaseLocationInfo(5,1,PawnColorType::White, PawnTypes::Queen) ,
	FPawnBaseLocationInfo(6,1,PawnColorType::White, PawnTypes::Bishop) ,
	FPawnBaseLocationInfo(7,1,PawnColorType::White, PawnTypes::Horse) ,
	FPawnBaseLocationInfo(8,1,PawnColorType::White, PawnTypes::Tour) ,
	FPawnBaseLocationInfo(1,2,PawnColorType::White, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(2,2,PawnColorType::White, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(3,2,PawnColorType::White, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(4,2,PawnColorType::White, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(5,2,PawnColorType::White, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(6,2,PawnColorType::White, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(7,2,PawnColorType::White, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(8,2,PawnColorType::White, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(1,8,PawnColorType::Black, PawnTypes::Tour) ,
	FPawnBaseLocationInfo(2,8,PawnColorType::Black, PawnTypes::Horse) ,
	FPawnBaseLocationInfo(3,8,PawnColorType::Black, PawnTypes::Bishop) ,
	FPawnBaseLocationInfo(4,8,PawnColorType::Black, PawnTypes::King) ,
	FPawnBaseLocationInfo(5,8,PawnColorType::Black, PawnTypes::Queen) ,
	FPawnBaseLocationInfo(6,8,PawnColorType::Black, PawnTypes::Bishop) ,
	FPawnBaseLocationInfo(7,8,PawnColorType::Black, PawnTypes::Horse) ,
	FPawnBaseLocationInfo(8,8,PawnColorType::Black, PawnTypes::Tour) ,
	FPawnBaseLocationInfo(1,7,PawnColorType::Black, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(2,7,PawnColorType::Black, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(3,7,PawnColorType::Black, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(4,7,PawnColorType::Black, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(5,7,PawnColorType::Black, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(6,7,PawnColorType::Black, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(7,7,PawnColorType::Black, PawnTypes::Pawn) ,
	FPawnBaseLocationInfo(8,7,PawnColorType::Black, PawnTypes::Pawn)  };
	
};
