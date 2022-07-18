// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "Actors/BoardCell.h"
#include "SChessGameModeBase.h"
#include "Actors/Pawns/BasePawn.h"
#include "PlayerDefPawn.generated.h"

/**
 * 
 */
UCLASS()
class SCHESS_API APlayerDefPawn : public ADefaultPawn
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	APlayerDefPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	ABoardCell* CurrentCellClicked;

	UPROPERTY()
	ABoardCell* PreviosCellClicked;

	UFUNCTION(BlueprintCallable)
	void OnMouseClicked();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UnHighlight();



private:

	UFUNCTION(NetMulticast, Reliable)
	void OnRep_AvailableCells(/*TMap<int32,int32> Cells*/);

	UFUNCTION()
	void FirstClick();

	UFUNCTION()
	void SecondClick();

	UFUNCTION(Server, Reliable)
	void FirstClickCall(int32 CellX, int32 CellY);

	UFUNCTION(Server, Reliable)
	void SecondClickCall(int32 FCellX, int32 FCellY, int32 SCellX, int32 SCellY);

	UFUNCTION()
	ABoardCell* GetClickedCell();

	UFUNCTION()
	void Clean();

	UPROPERTY(Replicated/*ReplicatedUsing = OnRep_AvailableCells*/)
	TArray<ABoardCell*> AvailableCells;

	UPROPERTY()
	ASChessGameModeBase* GameMode = nullptr;

	UPROPERTY(Replicated)
	bool bIsFisrtClick;
};
