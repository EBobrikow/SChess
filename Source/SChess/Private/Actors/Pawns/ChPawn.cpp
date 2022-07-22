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


void AChPawn::BeginPlay()
{
	Super::BeginPlay();
	bIsFirstMove = true;

	MovementPattern.Add(MovementDirections::Forward);
	MovementPattern.Add(MovementDirections::Forward_Left);
	MovementPattern.Add(MovementDirections::Forward_Right);

	MovementRange = MovementRange::OneStep;
}




