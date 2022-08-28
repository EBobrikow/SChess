// Fill out your copyright notice in the Description page of Project Settings.


#include "ChBasePlayerController.h"

TEnumAsByte<PawnColorType> AChBasePlayerController::GetControlledPawnsColor() const
{
	return ControlledPawnsColor;
}

void AChBasePlayerController::SetControlledPawnsColor(TEnumAsByte<PawnColorType> PawnsColor)
{
	ControlledPawnsColor = PawnsColor;
}

void AChBasePlayerController::SetControlledPawnLocation(FVector loc)
{
	if (ControlledPlayerPawn)
	{
		ControlledPlayerPawn->SetActorLocation(loc);
	}
}
