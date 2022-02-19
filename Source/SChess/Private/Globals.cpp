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
	: FPawnMovementInfo(0,0, PawnTypes::None, PawnColorType::None, false, 0, 0, PawnTypes::None, PawnColorType::None, false)
{
}

FPawnMovementInfo::FPawnMovementInfo(int32 FCindX, int32 FCindY, TEnumAsByte<PawnTypes> FCPawnType, TEnumAsByte<PawnColorType> FCPawnColor, bool FCIsFirstMove,
									 int32 SCindX, int32 SCindY, TEnumAsByte<PawnTypes> SCPawnType, TEnumAsByte<PawnColorType> SCPawnColor, bool SCIsFirstMove)
	: FirstCellIndexX(FCindX)
	, FirstCellIndexY(FCindY)
	, FirstCellPawnType(FCPawnType)
	, FirstCellPawnColor(FCPawnColor)
	, FirstCellIsFirstMove(FCIsFirstMove)
	, SecondCellIndexX(SCindX)
	, SecondCellIndexY(SCindY)
	, SecondCellPawnType(SCPawnType)
	, SecondCellPawnColor(SCPawnColor)
	, SecondCellIsFirstMove(SCIsFirstMove)
{
}
