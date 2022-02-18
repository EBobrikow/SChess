// Fill out your copyright notice in the Description page of Project Settings.


#include "Globals.h"

Globals::Globals()
{
}

Globals::~Globals()
{
}

FPawnBaseLocationInfo::FPawnBaseLocationInfo()
	: FPawnBaseLocationInfo(0,0, PawnColorType::None, PawnTypes::Pawn)
{
}

FPawnBaseLocationInfo::FPawnBaseLocationInfo(int32 X, int32 Y, TEnumAsByte<PawnColorType> SideColor, TEnumAsByte<PawnTypes> Type)
	: IndexX(X)
	, IndexY(Y)
	, PawnSideColor (SideColor)
	, PawnType(Type)
{
}

FPawnMovementInfo::FPawnMovementInfo()
	: FPawnMovementInfo(0,0, PawnTypes::None, PawnColorType::None, 0, 0, PawnTypes::None, PawnColorType::None)
{
}

FPawnMovementInfo::FPawnMovementInfo(int32 FCindX, int32 FCindY, TEnumAsByte<PawnTypes> FCPawnType, TEnumAsByte<PawnColorType> FCPawnColor,
									 int32 SCindX, int32 SCindY, TEnumAsByte<PawnTypes> SCPawnType, TEnumAsByte<PawnColorType> SCPawnColor)
	: FirstCellIndexX(FCindX)
	, FirstCellIndexY(FCindY)
	, FirstCellPawnType(FCPawnType)
	, FirstCellPawnColor(FCPawnColor)
	, SecondCellIndexX(SCindX)
	, SecondCellIndexY(SCindY)
	, SecondCellPawnType(SCPawnType)
	, SecondCellPawnColor(SCPawnColor)
{
}
