// Copyright Epic Games, Inc. All Rights Reserved.


#include "SChessGameModeBase.h"

ABasePawn* ASChessGameModeBase::GetPawnOnCellByIndex(int32 IndexX, int32 IndexY) const
{
	int32 X = 0, Y = 0;
	for (auto Cell : DeskArray)
	{
		Cell->GetIndex(X,Y);
		if (X == IndexX && Y == IndexY)
		{
			return Cell->GetPawnOnCell();
		}
	}

	return nullptr;
}

void ASChessGameModeBase::SetPawnOnCellByIndex(ABasePawn* PawnToSet, int32 IndexX, int32 IndexY)
{
	int32 X = 0, Y = 0;
	if (PawnToSet)
	{
		for (auto Cell : DeskArray)
		{
			Cell->GetIndex(X, Y);
			if (X == IndexX && Y == IndexY)
			{
				Cell->SetPawnOnCell(PawnToSet);
				break;
			}
		}
	
	}
}

ABoardCell* ASChessGameModeBase::GetCellByIndex(int32 X, int32 Y) const
{
	int32 indX = 0, indY = 0;
	for ( int32 i =0; i< DeskArray.Num(); i++)
	{
		DeskArray[i]->GetIndex(indX, indY);
		if (indX == X && indY == Y)
		{
			return DeskArray[i];
		}
	}

	return nullptr;
}

void ASChessGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CreateDesk();
	InitStartupArragment();
}

void ASChessGameModeBase::CreateDesk()
{
	FVector Location(-450.0f, -320.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	bool rightDir = true;
	int32 IndX = 0, IndY = 0;

	for (int32 i = 0; i < 64; i++)
	{
		ABoardCell* Cell = GetWorld()->SpawnActor<ABoardCell>(Location, Rotation, SpawnInfo);

		if (Cell)
		{
			if (BoardCellBlackMesh && BoardCellWhiteMesh)
			{
				if ((i + 1) % 2 == 0)
				{
					rightDir ? Cell->SetActorMesh(BoardCellWhiteMesh) : Cell->SetActorMesh(BoardCellBlackMesh);
				}
				else
				{
					rightDir ? Cell->SetActorMesh(BoardCellBlackMesh) : Cell->SetActorMesh(BoardCellWhiteMesh);
				}
					
			}
			
			Location.Y += 110.0f;
			IndX = (i + 1) % 8;
			IndX = IndX == 0 ? 8 : IndX;
			Cell->SetIndex(IndX, IndY +1);

			if ((i + 1) % 8 == 0)
			{
				Location.X += 110.0f;
				rightDir = !rightDir;
				Location.Y = -320.0f;
				IndY++;
			}
			DeskArray.Add(Cell);
		}
	}
}

void ASChessGameModeBase::SpawnChessPawn(ABoardCell* CellActorOnSpawn, TSubclassOf<ABasePawn> PawnClass, TEnumAsByte<PawnColorType> PawnColor)
{
	FVector PawnLocation(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	ABasePawn* ChessFigure = nullptr;

	if (CellActorOnSpawn)
	{
		PawnLocation = CellActorOnSpawn->GetActorLocation();
		if (PawnClass)
		{
			ChessFigure = GetWorld()->SpawnActor<ABasePawn>(PawnClass, PawnLocation, Rotation);

			if (ChessFigure)
			{
				ChessFigure->PawnColor = PawnColor;
				ChessFigure->InitFigure();
			}
		}

		CellActorOnSpawn->SetPawnOnCell(ChessFigure);
	}
}

void ASChessGameModeBase::InitStartupArragment()
{
	TSubclassOf<ABasePawn> tempPawnClass;
	FPawnBaseLocationInfo tempInfo;
	ABoardCell* tempCell = nullptr;

	for (int32 i = 0; i < TOTAL_FIGURES_NUM; i++)
	{
		tempInfo = InitialFiguresArrangment[i];
		switch (tempInfo.PawnType)
		{
		case PawnTypes::Pawn:
			tempPawnClass = ChessPawnClass;
			break;
		case PawnTypes::Bishop:
			tempPawnClass = ChessBishopClass;
			break;
		case PawnTypes::King:
			tempPawnClass = ChessKingClass;
			break;
		case PawnTypes::Tour:
			tempPawnClass = ChessTourClass;
			break;
		case PawnTypes::Horse:
			tempPawnClass = ChessHorseClass;
			break;
		case PawnTypes::Queen:
			tempPawnClass = ChessQueenClass;
			break;
		default:
			break;
		}

		tempCell = GetCellByIndex(tempInfo.IndexX, tempInfo.IndexY);
		if (tempCell && tempPawnClass)
			SpawnChessPawn(tempCell, tempPawnClass, tempInfo.PawnSideColor);

	}
}

void ASChessGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
