// Copyright Epic Games, Inc. All Rights Reserved.


#include "SChessGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Actors/Pawns/ChPawn.h"
#include <cstdlib>
#include "time.h"
#include "Actors/PlayerDefPawn.h"
#include "ChBasePlayerController.h"


void ASChessGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	

	/*APlayerController* PlayerControl = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerControl)
	{
		PlayerControl->bShowMouseCursor = true;
		PlayerControl->SetInputMode(FInputModeGameAndUI());
	}*/

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	MovementLogger = GetWorld()->SpawnActor<APawnMovementLogger>(Location, Rotation, SpawnInfo);
	if (MovementLogger)
	{
		OnPawnMoveDelegate.AddDynamic(MovementLogger,&APawnMovementLogger::LogPawnMovement);
	}

	OnMoveEndDelegate.AddDynamic(this, &ASChessGameModeBase::OnMoveEndEvent);
	OnPlayersConnectedDelegate.AddDynamic(this, &ASChessGameModeBase::OnPlayersReady);
	OnCheckDeclaredDelegate.AddDynamic(this, &ASChessGameModeBase::OnCheckDeclaredEvent);
	OnCheckMateDeclaredDelegate.AddDynamic(this, &ASChessGameModeBase::OnCheckMateDeclaredEvent);

	CurrentPawnsMove = PawnColorType::White;

	if (LoadingWidgetClass)
	{
		LoadingWidget = CreateWidget<ULoadingWidget>(GetWorld(), LoadingWidgetClass);
		if (LoadingWidget)
		{
			LoadingWidget->AddToViewport();
		}
	}

	
}

void ASChessGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerControllerList.Add(NewPlayer);
	if (PlayerControllerList.Num() == 2)
	{
		if (OnPlayersConnectedDelegate.IsBound())
		{
			OnPlayersConnectedDelegate.Broadcast();
		}
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

void ASChessGameModeBase::UndoLastMove()
{
	FPawnMovementInfo MoveInfo = FPawnMovementInfo();
	ABoardCell* FirstCell = nullptr;
	ABoardCell* SecondCell = nullptr;
	ABasePawn* tmpPawn = nullptr;
	if (MovementLogger)
	{
		MoveInfo = MovementLogger->GetLastMovementInfo();
		FirstCell = GetCellByIndex(MoveInfo.FirstCellIndexX, MoveInfo.FirstCellIndexY);
		SecondCell = GetCellByIndex(MoveInfo.SecondCellIndexX, MoveInfo.SecondCellIndexY);

		

		tmpPawn = SecondCell->GetPawnOnCell();
		FVector loc = FirstCell->GetActorLocation();
		tmpPawn->SetActorLocation(loc);
		tmpPawn->ConfigurePawn();
		tmpPawn->bIsFirstMove = MoveInfo.FirstCellIsFirstMove;
		tmpPawn->SetFoothold(FirstCell);
		FirstCell->SetPawnOnCell(tmpPawn);
		SecondCell->SetPawnOnCell(nullptr);

		if (MoveInfo.SecondCellPawnType != PawnTypes::None && MoveInfo.SecondCellPawnColor != PawnColorType::None)
		{
			SpawnChessPawn(SecondCell,GetSubclassOfPawnType(MoveInfo.SecondCellPawnType), MoveInfo.SecondCellPawnColor);
			SecondCell->GetPawnOnCell()->bIsFirstMove = MoveInfo.SecondCellIsFirstMove;
		}
		MovementLogger->RemoveLastMove();

		CurrentPawnsMove = CurrentPawnsMove == PawnColorType::White ? PawnColorType::Black : PawnColorType::White;
	}
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
		FPawnMovementInfo MoveInfo = FPawnMovementInfo();
		if (PawnOnPrevCell)
		{
			FromCell->GetIndex(MoveInfo.FirstCellIndexX, MoveInfo.FirstCellIndexY);
			MoveInfo.FirstCellPawnColor = PawnOnPrevCell->PawnColor;
			MoveInfo.FirstCellPawnType = PawnOnPrevCell->PawnType;
			ToCell->GetIndex(MoveInfo.SecondCellIndexX, MoveInfo.SecondCellIndexY);
			MoveInfo.FirstCellIsFirstMove = PawnOnPrevCell->bIsFirstMove;

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
						
						MoveInfo.SecondCellPawnColor = PawnOnCurCell->PawnColor;
						MoveInfo.SecondCellPawnType = PawnOnCurCell->PawnType;
						MoveInfo.SecondCellIsFirstMove = PawnOnCurCell->bIsFirstMove;

						PawnOnCurCell->SetFoothold(nullptr);
						if (PawnOnCurCell->PawnColor == PawnColorType::White)
							WhitePawns.Remove(PawnOnCurCell);
						if (PawnOnCurCell->PawnColor == PawnColorType::Black)
							BlackPawns.Remove(PawnOnCurCell);


						ToCell->SetPawnOnCell(nullptr);

						PawnOnCurCell->Destroy();
						if (GEngine)
							GEngine->ForceGarbageCollection(true);

					
					}
					
				}
				else
				{
					if (OnPawnMoveDelegate.IsBound())
					{
						OnPawnMoveDelegate.Broadcast(MoveInfo);
					}
					return;
				}
				

			}

			FVector loc = ToCell->GetActorLocation();
			PawnOnPrevCell->SetActorLocation(loc);
			PawnOnPrevCell->ConfigurePawn();
		
			PawnOnPrevCell->bIsFirstMove = false;
			PawnOnPrevCell->SetFoothold(ToCell);
			ToCell->SetPawnOnCell(PawnOnPrevCell);
			FromCell->SetPawnOnCell(nullptr);
			PawnOnPrevCell->isPawnProtected = false;

			if (OnPawnMoveDelegate.IsBound())
			{
				OnPawnMoveDelegate.Broadcast(MoveInfo);
			}

			PostMoveCheck(ToCell);

		}
	}
}

bool ASChessGameModeBase::IsPawnExistOnCell(int32 X, int32 Y, TEnumAsByte<PawnColorType> PawnColor, bool& IsSameColor)
{
	ABasePawn* tmpPawn = GetPawnOnCellByIndex(X, Y);
	if (tmpPawn)
	{
		IsSameColor = tmpPawn->PawnColor == PawnColor;
		if (IsSameColor)
		{
			tmpPawn->isPawnProtected = true;
		}
		
		
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

TArray<ABoardCell*> ASChessGameModeBase::GetForbiddenCellsForKing(TEnumAsByte<PawnColorType> KingColor)
{
	TArray<ABasePawn*> OpositePawns;
	TArray<ABoardCell*> tmpPawnMovement;
	TArray<ABoardCell*> OutSet;

	OpositePawns = KingColor == PawnColorType::Black ? WhitePawns : BlackPawns;

	for (ABasePawn* Pawn : OpositePawns)
	{
		tmpPawnMovement = Pawn->GetPossibleSteps(true);
		for (auto Cell : tmpPawnMovement)
		{
			OutSet.Remove(Cell);
			OutSet.Add(Cell);
		}
		
		if (Pawn->isPawnProtected)
		{
			OutSet.Remove(Pawn->GetFoohold());
			OutSet.Add(Pawn->GetFoohold());
		}
		
	}


	return OutSet;
}

bool ASChessGameModeBase::KingCanEscape(ABasePawn* PawnDeclaredCheck)
{
	TArray<ABoardCell*> ForbidenCells;
	TArray<ABoardCell*> AvaliableCells;
	TArray<ABoardCell*> tmpCells;
	TEnumAsByte<PawnColorType> KingColor = PawnDeclaredCheck->PawnColor == PawnColorType::White ? PawnColorType::Black : PawnColorType::White;
	ABasePawn* king = GetKingCell(KingColor)->GetPawnOnCell();
	tmpCells = king->GetPossibleSteps();

	UpdatePawnsProtectionState(PawnDeclaredCheck->PawnColor);

	for (auto Cell : tmpCells)
	{
		if (!Cell->GetPawnOnCell()->isPawnProtected)
		{
			AvaliableCells.Add(Cell);	
		}
	}

	if (AvaliableCells.Num() == 0)
		return false;

	ForbidenCells = GetForbiddenCellsForKing(KingColor);

	for (auto Cell : AvaliableCells)
	{
		
		for (auto forbidenCell : ForbidenCells)
		{
			AvaliableCells.Remove(forbidenCell);
		}
	}

	return AvaliableCells.Num() > 0;
}

bool ASChessGameModeBase::ThreatCanBeRemoved(ABasePawn* PawnDeclaredCheck)
{
	TArray<ABoardCell*> PawnAvailableCells;
	TArray<ABasePawn*> OpositePawns = PawnDeclaredCheck->PawnColor == PawnColorType::White ? BlackPawns : WhitePawns;

	for (ABasePawn* Pawn : OpositePawns)
	{
		if (Pawn->PawnType == PawnTypes::King && PawnDeclaredCheck->isPawnProtected)
		{
			continue;
		}

		PawnAvailableCells.Empty();
		PawnAvailableCells = Pawn->GetPossibleSteps();
		for (ABoardCell* Cell : PawnAvailableCells)
		{
			if (Cell == PawnDeclaredCheck->GetFoohold())
			{
				return true;
			}
		}
	}

	return false;
}

bool ASChessGameModeBase::KingCanBeProtected(ABasePawn* PawnDeclaredCheck)
{
	PawnColorType kingColor = PawnDeclaredCheck->PawnColor == PawnColorType::White ? PawnColorType::Black : PawnColorType::White;
	ABasePawn* King = GetKingCell(kingColor)->GetPawnOnCell();
	TArray<ABoardCell*> tmpArr = King->GetCellsBetweenPawns(PawnDeclaredCheck);
	TArray<ABoardCell*> moves;
	TArray<ABasePawn*> KingPawns = kingColor == PawnColorType::White ?  WhitePawns : BlackPawns;

	for (ABasePawn* Pawn : KingPawns)
	{
		if (Pawn == King)
		{
			continue;
		}

		moves.Empty();
		moves = Pawn->GetPossibleSteps();
		for (ABoardCell* PawnMoveCell : moves)
		{
			for (ABoardCell* BetweenCell : tmpArr)
			{
				if (PawnMoveCell == BetweenCell)
				{
					return true;
				}
			}
		}

	}

	return false;
}

void ASChessGameModeBase::UpdatePawnsProtectionState(TEnumAsByte<PawnColorType> PawnsColor)
{
	TArray<ABasePawn*> SamePawns = PawnsColor == PawnColorType::White ? WhitePawns : BlackPawns;
	for (ABasePawn* Pawn : SamePawns)
	{
		Pawn->GetPossibleSteps();
	}
}


TEnumAsByte<PawnColorType> ASChessGameModeBase::GetCurrentPawnsMove() const
{
	return CurrentPawnsMove;
}

void ASChessGameModeBase::SetCurrentPawnsMove(TEnumAsByte<PawnColorType> CurrentPawns)
{
	CurrentPawnsMove = CurrentPawns;
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
				//ChessFigure->SetColor();

				if (ChessFigure->PawnColor == PawnColorType::White)
				{
					WhitePawns.Add(ChessFigure);
				}
				else if (ChessFigure->PawnColor == PawnColorType::Black)
				{
					BlackPawns.Add(ChessFigure);
				}

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
		tempPawnClass = GetSubclassOfPawnType(tempInfo.PawnType);

		tempCell = GetCellByIndex(tempInfo.IndexX, tempInfo.IndexY);
		if (tempCell && tempPawnClass)
			SpawnChessPawn(tempCell, tempPawnClass, tempInfo.PawnSideColor);

	}
}

void ASChessGameModeBase::OnMoveEndEvent()
{
	CurrentPawnsMove = GetCurrentPawnsMove() == PawnColorType::White ? PawnColorType::Black : PawnColorType::White;
}

void ASChessGameModeBase::OnPlayersReady()
{
	srand(rand());
	//float remain = rand() / 2.0f;
	bool devid = (rand() / 2.0f) == 0.0f;
	FVector blackLoc = FVector(720.0f,65.0f,380.0f);
	FVector whiteLoc = FVector(-800.0f, 65.0f, 360.0f);

	AChBasePlayerController* pl1 = Cast<AChBasePlayerController>(PlayerControllerList[0]);
	if (pl1)
	{
		pl1->SetControlledPawnsColor(PawnColorType::White);//devid? PawnColorType::White : PawnColorType::Black);
		
		//pl1->SetControlledPawnLocation(devid ? whiteLoc : blackLoc);
	}

	AChBasePlayerController* pl2 = Cast<AChBasePlayerController>(PlayerControllerList[1]);
	if (pl2)
	{
		pl2->SetControlledPawnsColor(PawnColorType::Black);//devid ? PawnColorType::Black : PawnColorType::White);
		//pl1->SetControlledPawnLocation(devid ? blackLoc : whiteLoc);
	}

	CreateDesk();
	InitStartupArragment();

	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromViewport();
	}
}

ABoardCell* ASChessGameModeBase::GetKingCell(TEnumAsByte<PawnColorType> KingColor)
{
	TArray<ABasePawn*> AllPawns;
	AllPawns = KingColor == PawnColorType::Black ?  BlackPawns : WhitePawns;

	for (ABasePawn* Pawn : AllPawns)
	{
		if (Pawn->PawnType == PawnTypes::King)
		{
			return Pawn->GetFoohold();
		}
	}

	return nullptr;
}

ABasePawn* ASChessGameModeBase::GetPawnDeclaredCheckForKing(TEnumAsByte<PawnColorType> KingColor)
{
	TArray<ABasePawn*> AllPawns;
	TArray<ABoardCell*> PossibleSteps;
	AllPawns = KingColor == PawnColorType::Black ? WhitePawns : BlackPawns;

	for (ABasePawn* Pawn : AllPawns)
	{
		PossibleSteps.Empty();
		PossibleSteps = Pawn->GetPossibleSteps(true);

		for (ABoardCell* Cell : PossibleSteps)
		{
			if (!Cell->GetPawnOnCell()) // if there is no pawn on cell skip this cell
			{
				continue;
			}
			else
			{
				if (Cell->GetPawnOnCell()->PawnType == PawnTypes::King && Cell->GetPawnOnCell()->PawnColor == KingColor)
				{
					return Pawn;
				}
			}
		}

	}

	return nullptr;
}

bool ASChessGameModeBase::CheckExamination(TEnumAsByte<PawnColorType> KingColor)
{
	TArray<ABoardCell*>  PossibleSteps = GetForbiddenCellsForKing(KingColor);

	for (auto Cell : PossibleSteps)
	{
		if (Cell->GetPawnOnCell() && Cell->GetPawnOnCell()->PawnType == PawnTypes::King && Cell->GetPawnOnCell()->PawnColor == KingColor)
		{

			if (CheckState)
				UndoLastMove();
			else
				CheckState = true;

			if (OnCheckDeclaredDelegate.IsBound())
			{
				OnCheckDeclaredDelegate.Broadcast(GetPawnDeclaredCheckForKing(KingColor));
			}

			return true;


		}
	}
	
	return false;
}

void ASChessGameModeBase::OnCheckDeclaredEvent(ABasePawn* PawnDeclaredCheck)
{
	UE_LOG(LogTemp, Warning, TEXT("Check!"));

	if (!KingCanEscape(PawnDeclaredCheck) && !ThreatCanBeRemoved(PawnDeclaredCheck) && !KingCanBeProtected(PawnDeclaredCheck))
	{
		UE_LOG(LogTemp, Warning, TEXT("Check&Mate!"));
		if (OnCheckMateDeclaredDelegate.IsBound())
		{
			OnCheckMateDeclaredDelegate.Broadcast();
		}
	}

	if (OnMoveEndDelegate.IsBound())
	{
		OnMoveEndDelegate.Broadcast();
	}
}

void ASChessGameModeBase::OnCheckMateDeclaredEvent()
{
}



TSubclassOf<ABasePawn> ASChessGameModeBase::GetSubclassOfPawnType(TEnumAsByte<PawnTypes> Type)
{
	switch (Type)
	{
	case PawnTypes::Pawn:
		return ChessPawnClass;
		break;
	case PawnTypes::Bishop:
		return ChessBishopClass;
		break;
	case PawnTypes::King:
		return ChessKingClass;
		break;
	case PawnTypes::Tour:
		return ChessTourClass;
		break;
	case PawnTypes::Horse:
		return ChessHorseClass;
		break;
	case PawnTypes::Queen:
		return ChessQueenClass;
		break;
	default:
		break;
	}
	return TSubclassOf<ABasePawn>();
}

void ASChessGameModeBase::PostMoveCheck(ABoardCell* FinalCell)
{

	if (CheckExamination(PawnColorType::Black)) return;
	if (CheckExamination(PawnColorType::White)) return;

	CheckState = false;

	if (OnMoveEndDelegate.IsBound())
	{
		OnMoveEndDelegate.Broadcast();
	}

}


