// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PlayerDefPawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ChBasePlayerController.h"


void APlayerDefPawn::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//checkf(GameMode, TEXT("APlayerDefPawn::BeginPlay GameMode is invalid"));

	bIsFisrtClick = true;
	CurrentCellClicked = nullptr;
	PreviosCellClicked = nullptr;

	APlayerController* PlayerControl = UGameplayStatics::GetPlayerController(GetOwner(), 0); 
	if (PlayerControl)
	{
		PlayerControl->bShowMouseCursor = true;
		PlayerControl->SetInputMode(FInputModeGameAndUI());
	}
	

}

void APlayerDefPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerDefPawn, bIsFisrtClick);
	DOREPLIFETIME_CONDITION(APlayerDefPawn, AvailableCells, COND_InitialOrOwner);
	//DOREPLIFETIME(APlayerDefPawn, AvailableCells);

}


APlayerDefPawn::APlayerDefPawn()
{
	bReplicates = true;
	SetReplicateMovement(true);
}

void APlayerDefPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerDefPawn::OnMouseClicked()
{
	if (bIsFisrtClick)
		FirstClick();
	else
		SecondClick();

}

void APlayerDefPawn::UnHighlight_Implementation()
{
	ASChessGameModeBase* GM = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
		GM->UnHighlightAll();

	if (AvailableCells.Num() > 0)
	{
		for (auto cell : AvailableCells)
		{
			cell->bIsHighlighted = false;//MeshComponent->SetRenderCustomDepth(true);
		}
		
	}
}



void APlayerDefPawn::OnRep_AvailableCells_Implementation(/*TMap<int32, int32> Cells*/)
{
	
	if (AvailableCells.Num() > 0)
	{
		for (auto cell : AvailableCells)
		{
			cell->MeshComponent->SetRenderCustomDepth(true);
		}
		bIsFisrtClick = false;
	}
}

/*If there is pawn on cell, show possible cells to move*/
void APlayerDefPawn::FirstClick()
{

	CurrentCellClicked = GetClickedCell();
	int32 X = 0, Y = 0;
	if (CurrentCellClicked)
	{
		CurrentCellClicked->GetIndex(X, Y);
		FirstClickCall(X, Y);
	}
	

}

/*Finish moving chess pawn*/
void APlayerDefPawn::SecondClick()
{
	int32 X = 0, Y = 0, CurX = 0, CurY = 0;

	PreviosCellClicked = CurrentCellClicked;
	CurrentCellClicked  = GetClickedCell();
	if (PreviosCellClicked && CurrentCellClicked)
	{
		PreviosCellClicked->GetIndex(X, Y);
		CurrentCellClicked->GetIndex(CurX, CurY);

		SecondClickCall(X, Y, CurX, CurY);
	}
	

	
	Clean();
	bIsFisrtClick = true;
}

void APlayerDefPawn::FirstClickCall_Implementation(int32 CellX, int32 CellY)
{
	int32 X = 0, Y = 0;
	TArray<ABoardCell*> ProtectedCells;
	GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AChBasePlayerController* pl1 = Cast<AChBasePlayerController>(GetController());
	if (pl1)
	{
		if (GameMode->GetCurrentPawnsMove() != pl1->GetControlledPawnsColor())
		{
			return;
		}
		
	}

	ABoardCell* LocalCell = GameMode->GetCellByIndex(CellX, CellY);
	if (LocalCell)
	{

		if (GEngine)
		{
			FString msg = "";
			LocalCell->GetName(msg);//FString::Printf(TEXT(""));
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, msg);
		}

		if (LocalCell->GetPawnOnCell())
		{
			AvailableCells = LocalCell->GetPawnOnCell()->GetPossibleSteps();

			if (LocalCell->GetPawnOnCell()->PawnType == PawnTypes::King)
			{
				TArray<ABoardCell*> tmpCells = GameMode->GetForbiddenCellsForKing(LocalCell->GetPawnOnCell()->PawnColor);

				for (auto tmpCell : tmpCells)
				{
					AvailableCells.Remove(tmpCell);
				}

				/*for (auto Cell : AvailableCells)
				{
					if (ABasePawn* localPawn = Cell->GetPawnOnCell())
					{
						if (localPawn->isPawnProtected)
						{
							ProtectedCells.Add(Cell);
						}
					}
				}

				for (auto cell : ProtectedCells)
				{
					AvailableCells.Remove(cell);
				}*/
			}
		}
		

		if (AvailableCells.Num() > 0)
		{
			OnRep_AvailableCells();
			bIsFisrtClick = false;
		}

	}
}

void APlayerDefPawn::SecondClickCall_Implementation(int32 FCellX, int32 FCellY, int32 SCellX, int32 SCellY)
{
	int32 X = 0, Y = 0;
	GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	PreviosCellClicked = GameMode->GetCellByIndex(FCellX, FCellY);
	CurrentCellClicked = GameMode->GetCellByIndex(SCellX, SCellY);

	if (CurrentCellClicked && PreviosCellClicked && PreviosCellClicked != CurrentCellClicked)
	{
		//CurrentCellClicked->GetIndex(CurX, CurY);
		if (GameMode)
		{
			if (AvailableCells.Num() > 0)
			{
				for (auto cell : AvailableCells)
				{
					cell->GetIndex(X, Y);
					if (X == SCellX && Y == SCellY)
					{
						GameMode->StartMovePawn(PreviosCellClicked, CurrentCellClicked);
						Clean();
						bIsFisrtClick = true;
						return;
					}

				}
			}



		}
	}
	bIsFisrtClick = true;
}

/*Return a chess cell that was clicked*/
ABoardCell* APlayerDefPawn::GetClickedCell()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());// UGameplayStatics::GetPlayerController(GetOwner(), 0);
	if (PlayerController)
	{
		FHitResult HitResult;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectsType;
		objectsType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		PlayerController->GetHitResultUnderCursorForObjects(objectsType, true, HitResult);

		if (HitResult.bBlockingHit)
		{
			auto tmpPawn = Cast<ABasePawn>(HitResult.Actor);
			if (tmpPawn)
			{
				return tmpPawn->GetFoohold();
			}
			

			return Cast<ABoardCell>(HitResult.Actor);
		}
	}

	return nullptr;
}

void APlayerDefPawn::Clean()
{
	PreviosCellClicked = nullptr;
	CurrentCellClicked = nullptr;
	
	for (auto cell : AvailableCells)
	{
		
		cell->MeshComponent->SetRenderCustomDepth(false); //bIsHighlighted = false;
	}

	UnHighlight();
	AvailableCells.Empty();
}
