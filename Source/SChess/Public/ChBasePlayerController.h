// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Globals.h"
#include "Actors/PlayerDefPawn.h"
#include "ChBasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SCHESS_API AChBasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION()
	TEnumAsByte<PawnColorType> GetControlledPawnsColor() const;

	UFUNCTION()
	void SetControlledPawnsColor(TEnumAsByte<PawnColorType> PawnsColor);

	//UPROPERTY()
	//FVector location;

	UPROPERTY()
	ADefaultPawn* ControlledPlayerPawn;

	UFUNCTION()
	void SetControlledPawnLocation(FVector loc);


protected:

	TEnumAsByte<PawnColorType> ControlledPawnsColor;
	
};
