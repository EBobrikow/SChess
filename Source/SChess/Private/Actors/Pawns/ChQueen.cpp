// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pawns/ChQueen.h"

void AChQueen::BeginPlay()
{
	Super::BeginPlay();

	MovementPattern.Add(MovementDirections::Forward_Left);
	MovementPattern.Add(MovementDirections::Forward_Right);
	MovementPattern.Add(MovementDirections::Back_Left);
	MovementPattern.Add(MovementDirections::Back_Right);

	MovementPattern.Add(MovementDirections::Forward);
	MovementPattern.Add(MovementDirections::Left);
	MovementPattern.Add(MovementDirections::Back);
	MovementPattern.Add(MovementDirections::Right);

	MovementRange = MovementRange::Infinite;
}

void AChQueen::ConfigurePawn()
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
