// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PlayerDefPawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


void APlayerDefPawn::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	checkf(GameMode, TEXT("APlayerDefPawn::BeginPlay GameMode is invalid"));

	bIsFisrtClick = true;
	CurrentCellClicked = nullptr;
	PreviosCellClicked = nullptr;
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

/*If there is pawn on cell, show possible cells to move*/
void APlayerDefPawn::FirstClick()
{

	CurrentCellClicked = GetClickedCell();

	if (CurrentCellClicked)
	{

		if (GEngine)
		{
			FString msg = "";
			CurrentCellClicked->GetName(msg);//FString::Printf(TEXT(""));
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, msg);
		}

		if (CurrentCellClicked->GetPawnOnCell())
		{
			AvailableCells = CurrentCellClicked->GetPawnOnCell()->GetPossibleSteps();

			if (CurrentCellClicked->GetPawnOnCell()->PawnType == PawnTypes::King)
			{
				TArray<ABoardCell*> tmpCells = GameMode->GetForbiddenCellsForKing(CurrentCellClicked->GetPawnOnCell()->PawnColor);

				for (auto tmpCell : tmpCells)
				{
					AvailableCells.Remove(tmpCell);
				}
			}
		}

		

		if (AvailableCells.Num() > 0)
		{
			for (auto cell : AvailableCells)
			{
				cell->MeshComponent->SetRenderCustomDepth(true);
			}
			bIsFisrtClick = false;
		}

	}
}

/*Finish moving chess pawn*/
void APlayerDefPawn::SecondClick()
{
	int32 X = 0, Y = 0, CurX = 0, CurY = 0;

	PreviosCellClicked = CurrentCellClicked;
	CurrentCellClicked = CurrentCellClicked = GetClickedCell();
	

	if (CurrentCellClicked && PreviosCellClicked && PreviosCellClicked != CurrentCellClicked)
	{
		CurrentCellClicked->GetIndex(CurX, CurY);
		if (GameMode)
		{
			if (AvailableCells.Num() > 0)
			{
				for (auto cell : AvailableCells)
				{
					cell->GetIndex(X, Y);
					if (X == CurX && Y == CurY)
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
	Clean();
	bIsFisrtClick = true;
}

/*Return a chess cell that was clicked*/
ABoardCell* APlayerDefPawn::GetClickedCell()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FHitResult HitResult;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectsType;
		objectsType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		PlayerController->GetHitResultUnderCursorForObjects(objectsType, true, HitResult);

		if (HitResult.bBlockingHit)
		{
			return Cast<ABoardCell>(HitResult.Actor);
		}
	}

	return nullptr;
}

void APlayerDefPawn::Clean()
{
	PreviosCellClicked = nullptr;
	CurrentCellClicked = nullptr;
	GameMode->UnHighlightAll();
	AvailableCells.Empty();
}
