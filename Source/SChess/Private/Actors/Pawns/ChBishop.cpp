// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pawns/ChBishop.h"

void AChBishop::BeginPlay()
{
	Super::BeginPlay();

	MovementPattern.Add(MovementDirections::Forward_Left);
	MovementPattern.Add(MovementDirections::Forward_Right);
	MovementPattern.Add(MovementDirections::Back_Left);
	MovementPattern.Add(MovementDirections::Back_Right);

	MovementRange = MovementRange::Infinite;
}

void AChBishop::ConfigurePawn()
{
	FVector PawnLocation(0.0f, 0.0f, 0.0f);
	FVector Scale(40.0f, 40.0f, 40.0f);

	FRotator Rotation(0.0f, 0.0f, 90.0f);


	if (MeshComponent)
	{
		MeshComponent->SetRelativeScale3D(Scale);
	}
	PawnLocation = this->GetActorLocation();
	PawnLocation.Z += 40;

	this->SetActorLocation(PawnLocation);
	this->SetActorRotation(Rotation);
}

//TArray<ABoardCell*> AChBishop::GetPossibleSteps()
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
//
//	ASChessGameModeBase* GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
//	Cell->GetIndex(CurX, CurY);
//
//
//
//	return OutArray;
//}
