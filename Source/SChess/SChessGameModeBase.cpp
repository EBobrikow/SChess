// Copyright Epic Games, Inc. All Rights Reserved.


#include "SChessGameModeBase.h"
#include "Actors/Pawns/ChPawn.h"

void ASChessGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CreateDesk();
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

		if (Cell)// && BoardCellMesh)
		{
			
			if (BoardCellBlackMesh && BoardCellWhiteMesh)
			{
				if ((i + 1) % 2 == 0)
				{
					rightDir ? Cell->SetActorMesh(BoardCellBlackMesh) : Cell->SetActorMesh(BoardCellWhiteMesh);
					//Cell->SetActorMesh(BoardCellBlackMesh);
				}
				else
				{
					rightDir ? Cell->SetActorMesh(BoardCellWhiteMesh) : Cell->SetActorMesh(BoardCellBlackMesh);
					//Cell->SetActorMesh(BoardCellWhiteMesh);
				}
					
			}
			
			//if (rightDir)
			Location.X += 110.0f;
			//else 
				//Location.X -= 110.0f;

			IndX = (i + 1) % 8;
			IndX = IndX == 0 ? 8 : IndX;
			Cell->SetIndex(IndX, IndY +1);

			if ((i + 1) % 8 == 0)
			{
				Location.Y += 110.0f;
				rightDir = !rightDir;
				Location.X = -450.0f;//0.0f;
				IndY++;
				//IndX = 0;
			}
			DeskArray.Add(Cell);


			//IndX++;
		}

		
		
	}

	FVector PawnLocation(0.0f, 0.0f, 100.0f);
	FRotator RotationCh(0.0f, 0.0f, 90.0f);
	ABasePawn* ChessFigure = nullptr;
	ABasePawn* ChessFigure1 = nullptr;
	if (DeskArray.Num() > 0)
	{
		PawnLocation = DeskArray[0]->GetActorLocation();
		//PawnLocation.Z += 50;
		if (ChessPawnClass)
			ChessFigure = GetWorld()->SpawnActor<AChPawn>(ChessPawnClass, PawnLocation, RotationCh);

		if (ChessFigure)
		{
			ChessFigure->InitFigure();
			
		}
		PawnLocation = DeskArray[1]->GetActorLocation();
		ChessFigure1 = GetWorld()->SpawnActor<AChPawn>(ChessPawnClass, PawnLocation, RotationCh);
		if (ChessFigure1)
		{
			ChessFigure1->PawnColor = PawnColorType::White;
			ChessFigure1->InitFigure();
		}
		
	}
	

}

void ASChessGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
