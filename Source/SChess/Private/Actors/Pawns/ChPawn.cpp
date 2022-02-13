// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pawns/ChPawn.h"



void AChPawn::ConfigurePawn()
{
	FVector Scale(27.0f, 4.0f, 27.0f);
	FRotator Rotation(0.0f, 0.0f, 90.0f);
	if (MeshComponent)
	{
		MeshComponent->SetRelativeScale3D(Scale);
	}
	
	this->SetActorRotation(Rotation);
}

AChPawn::AChPawn()
{

}

//TArray<ABoardCell*> AChPawn::GetPossibleSteps()
//{
//	int32 CurX = 0, CurY = 0;
//	ABoardCell* Cell = GetFoohold();
//	ABoardCell* tmpCell = nullptr;
//	ABasePawn* tmpPawn = nullptr;
//	TArray<ABoardCell*> OutArray;
//	bool DetectedPawnSameColor = false;
//	int32 moveDirectionX = 0, moveDirectionY = 0;
//
//	moveDirectionX = (PawnColor == PawnColorType::White) ? 1 : -1;
//	moveDirectionY = (PawnColor == PawnColorType::White) ? 1 : -1;
//	OutArray.Add(Cell);
//	ASChessGameModeBase* GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
//	Cell->GetIndex(CurX, CurY);
//	if (GameMode)
//	{
//		for (auto Direction : MovementPattern)
//		{
//			switch (Direction)
//			{
//			case MovementDirections::Forward:
//				if (CurY+ moveDirectionY <= 8 && CurY + moveDirectionY > 0)
//				{
//					if (!GameMode->IsPawnExistOnCell(CurX , CurY + moveDirectionY, PawnColor, DetectedPawnSameColor))
//					{
//						OutArray.Add(GameMode->GetCellByIndex(CurX , CurY + moveDirectionY));
//					}
//				}
//
//				if (bIsFirstMove && PawnType == PawnTypes::Pawn)
//				{
//					if (CurY + moveDirectionY + moveDirectionY <= 8 && CurY + moveDirectionY + moveDirectionY > 0)
//					{
//						if (!GameMode->IsPawnExistOnCell(CurX, CurY + moveDirectionY + moveDirectionY, PawnColor, DetectedPawnSameColor))
//						{
//							OutArray.Add(GameMode->GetCellByIndex(CurX, CurY + moveDirectionY + moveDirectionY));
//						}
//					}
//				}
//				break;
//
//			case MovementDirections::Atack_Forward_Left:
//				if (CurX + moveDirectionX <= 8 && CurX + moveDirectionX > 0 && CurY - moveDirectionY <=8 && CurY - moveDirectionY > 0)
//				{
//					if (GameMode->IsPawnExistOnCell(CurX - moveDirectionX, CurY + moveDirectionY, PawnColor, DetectedPawnSameColor))
//					{
//						if (!DetectedPawnSameColor)
//						{
//							OutArray.Add(GameMode->GetCellByIndex(CurX - moveDirectionX, CurY + moveDirectionY));
//						}
//
//					}
//				}
//
//
//				break;
//
//			case MovementDirections::Atack_Forward_Right:
//				if (CurX + moveDirectionX <= 8 && CurX + moveDirectionX > 0 && CurY - moveDirectionY <= 8 && CurY - moveDirectionY > 0)
//				{
//					if (GameMode->IsPawnExistOnCell(CurX + moveDirectionX, CurY + moveDirectionY, PawnColor, DetectedPawnSameColor))
//					{
//						if (!DetectedPawnSameColor)
//						{
//							OutArray.Add(GameMode->GetCellByIndex(CurX + moveDirectionX, CurY + moveDirectionY));
//						}
//
//					}
//				}
//				break;
//
//			default:
//				break;
//			}
//		}
//	}
//	
//
//	return OutArray;
//}

void AChPawn::BeginPlay()
{
	Super::BeginPlay();
	bIsFirstMove = true;

	MovementPattern.Add(MovementDirections::Forward);
	MovementPattern.Add(MovementDirections::Atack_Forward_Left);
	MovementPattern.Add(MovementDirections::Atack_Forward_Right);

	MovementRange = MovementRange::OneStep;
}




