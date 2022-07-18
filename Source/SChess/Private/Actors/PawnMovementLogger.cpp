// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PawnMovementLogger.h"

// Sets default values
APawnMovementLogger::APawnMovementLogger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APawnMovementLogger::LogPawnMovement(FPawnMovementInfo MovementInfo)
{
	UE_LOG(LogTemp, Warning,TEXT("%s"), *MovementLogToString(MovementInfo));
	MovementLogList.Add(MovementInfo);
}

FPawnMovementInfo APawnMovementLogger::GetLastMovementInfo() const
{
	if (MovementLogList.Num() >0)
	{
		return MovementLogList[MovementLogList.Num() - 1];
	}
	return FPawnMovementInfo();
}

void APawnMovementLogger::RemoveLastMove()
{
	if (MovementLogList.Num() > 0)
	{
		MovementLogList.RemoveAt(MovementLogList.Num() - 1);
	}
}

// Called when the game starts or when spawned
void APawnMovementLogger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnMovementLogger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString APawnMovementLogger::MovementLogToString(FPawnMovementInfo MovInfo) const
{
	FString res = FString::Printf(TEXT("%s %s %s%i "), *PawnColorToString(MovInfo.FirstCellPawnColor), *PawnTypeToString(MovInfo.FirstCellPawnType), *IndXtoString(MovInfo.FirstCellIndexX), MovInfo.FirstCellIndexY);

	if (MovInfo.SecondCellPawnType != PawnTypes::None && MovInfo.SecondCellPawnColor != PawnColorType::None)
	{
		res += FString::Printf(TEXT("beat %s %s %s%i"), *PawnColorToString(MovInfo.SecondCellPawnColor), *PawnTypeToString(MovInfo.SecondCellPawnType), *IndXtoString(MovInfo.SecondCellIndexX), MovInfo.SecondCellIndexY);
	}
	else
	{
		res += FString::Printf(TEXT("move %s%i"), *IndXtoString(MovInfo.SecondCellIndexX), MovInfo.SecondCellIndexY);
	}

	return res;
}

FString APawnMovementLogger::PawnColorToString(TEnumAsByte<PawnColorType> Color) const
{
	switch (Color)
	{
	case PawnColorType::White:
		return "White";
		break;

	case PawnColorType::Black:
		return "Black";
		break;

	default:
		return "";
		break;
	}
	
}

FString APawnMovementLogger::PawnTypeToString(TEnumAsByte<PawnTypes> Type) const
{
	switch (Type)
	{
	case PawnTypes::Pawn:
		return "Pawn";
		break;

	case PawnTypes::Bishop:
		return "Bishop";
		break;

	case PawnTypes::Horse:
		return "Horse";
		break;

	case PawnTypes::King:
		return "King";
		break;

	case PawnTypes::Queen:
		return "Queen";
		break;

	case PawnTypes::Tour:
		return "Tour";
		break;

	default:
		return "";
		break;
	}


}

FString APawnMovementLogger::IndXtoString(int32 IndexX) const
{
	switch (IndexX)
	{
	case 1:
		return "A";
		break;

	case 2:
		return "B";
		break;

	case 3:
		return "C";
		break;

	case 4:
		return "D";
		break;

	case 5:
		return "E";
		break;

	case 6:
		return "F";
		break;

	case 7:
		return "G";
		break;

	case 8:
		return "H";
		break;
	default:
		return "";
		break;
	}
	
}

