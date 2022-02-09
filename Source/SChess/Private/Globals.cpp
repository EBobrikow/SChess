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
