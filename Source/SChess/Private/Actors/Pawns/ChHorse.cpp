// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pawns/ChHorse.h"

void AChHorse::BeginPlay()
{
	Super::BeginPlay();

	MovementPattern.Add(MovementDirections::T_Forward_Right);
	MovementPattern.Add(MovementDirections::T_Forward_Left);
	MovementPattern.Add(MovementDirections::T_Left_Right);
	MovementPattern.Add(MovementDirections::T_Left_Left);
	MovementPattern.Add(MovementDirections::T_Back_Right);
	MovementPattern.Add(MovementDirections::T_Back_Left);
	MovementPattern.Add(MovementDirections::T_Right_Right);
	MovementPattern.Add(MovementDirections::T_Right_Left);

	MovementRange = MovementRange::OneStep;
}

void AChHorse::ConfigurePawn()
{
	FVector PawnLocation(0.0f, 0.0f, 0.0f);
	FVector Scale(40.0f, 40.0f, 40.0f);
	FRotator Rotation(0.0f, -90.0f, 90.0f);
	if (PawnColor == PawnColorType::Black)
		Rotation.Yaw = 90.0f;
	if (MeshComponent)
	{
		MeshComponent->SetRelativeScale3D(Scale);
	}
	PawnLocation = this->GetActorLocation();
	PawnLocation.Z += 40;

	this->SetActorLocation(PawnLocation);
	this->SetActorRotation(Rotation);
}
