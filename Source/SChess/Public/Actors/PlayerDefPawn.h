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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	ABoardCell* CurrentCellClicked;

	UPROPERTY()
	ABoardCell* PreviosCellClicked;

	UFUNCTION(BlueprintCallable)
	void OnMouseClicked();

private:

	UFUNCTION()
	void FirstClick();

	UFUNCTION()
	void SecondClick();

	UFUNCTION()
	ABoardCell* GetClickedCell();

	UFUNCTION()
	void Clean();

	UPROPERTY()
	TArray<ABoardCell*> AvailableCells;

	UPROPERTY()
	ASChessGameModeBase* GameMode = nullptr;

	bool bIsFisrtClick;
};
