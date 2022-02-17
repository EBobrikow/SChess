// Copyright Epic Games, Inc. All Rights Reserved.


#include "SChessGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Actors/Pawns/ChPawn.h"


void ASChessGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CreateDesk();
	InitStartupArragment();

	APlayerController* PlayerControl = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerControl)
	{
		PlayerControl->bShowMouseCursor = true;
		PlayerControl->SetInputMode(FInputModeGameAndUI());
	}


}


void ASChessGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

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

void ASChessGameModeBase::AddDynaMatForPostProcessVolume()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, APostProcessVolume::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		APostProcessVolume* PPV = Cast<APostProcessVolume>(Actors[0]);
		if (PPV)
		{
			FPostProcessSettings& PostProcessSettings = PPV->Settings;
			if (HighlightMaterial)
			{
				DynamicHighlightMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, HighlightMaterial);
				FWeightedBlendable WeightedBlendable;
				WeightedBlendable.Object = DynamicHighlightMaterial;
				WeightedBlendable.Weight = 1;

				PostProcessSettings.WeightedBlendables.Array.Add(WeightedBlendable);
			}
		}
	}
}

void ASChessGameModeBase::UpdatePostProcessMaterial(int32 X, int32 Y, int32 Z)
{
	if (DynamicHighlightMaterial)
	{
		FLinearColor NewColor(X, Y, Z, 0.0f);
		DynamicHighlightMaterial->SetVectorParameterValue(TEXT("HighlightColor"), NewColor);
	}
}

void ASChessGameModeBase::StartMovePawn(ABoardCell* FromCell, ABoardCell* ToCell)
{
	if (FromCell && ToCell && FromCell != ToCell)
	{
		ABasePawn* PawnOnPrevCell = FromCell->GetPawnOnCell();
		ABasePawn* PawnOnCurCell = ToCell->GetPawnOnCell();
		if (PawnOnPrevCell)
		{
			if (PawnOnCurCell )
			{
				if (PawnOnPrevCell->PawnColor != PawnOnCurCell->PawnColor)
				{
					if (PawnOnCurCell->PawnType == PawnTypes::King)
					{
						UE_LOG(LogTemp, Warning, TEXT("Impossible to destroy King pawn"));
						return;
					}
					else
					{
						PawnOnCurCell->SetFoothold(nullptr);
						PawnOnCurCell->Destroy();
						ToCell->SetPawnOnCell(nullptr);
						if (GEngine)
							GEngine->ForceGarbageCollection(true);
					}
					
				}
				else
				{
					return;
				}
				

			}

			FVector loc = ToCell->GetActorLocation();
			PawnOnPrevCell->SetActorLocation(loc);
			PawnOnPrevCell->ConfigurePawn();
			/*if (PawnOnPrevCell->PawnType == PawnTypes::Pawn)
			{
				AChPawn* ChPawn = Cast<AChPawn>(PawnOnPrevCell);
				ChPawn->bIsFirstMove = false;
			}*/
			PawnOnPrevCell->bIsFirstMove = false;
			PawnOnPrevCell->SetFoothold(ToCell);
			ToCell->SetPawnOnCell(PawnOnPrevCell);
			FromCell->SetPawnOnCell(nullptr);

		}
	}
}

bool ASChessGameModeBase::IsPawnExistOnCell(int32 X, int32 Y, TEnumAsByte<PawnColorType> PawnColor, bool& IsSameColor)
{
	ABasePawn* tmpPawn = GetPawnOnCellByIndex(X, Y);
	if (tmpPawn)
	{
		IsSameColor = tmpPawn->PawnColor == PawnColor;
		return true;
	}	
	else
		return false;
}

void ASChessGameModeBase::UnHighlightAll()
{
	for (auto cell : DeskArray)
	{
		cell->MeshComponent->SetRenderCustomDepth(false);
	}
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
				ChessFigure->SetFoothold(CellActorOnSpawn);
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

