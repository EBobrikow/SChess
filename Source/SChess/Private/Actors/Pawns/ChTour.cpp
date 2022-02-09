// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pawns/ChTour.h"

void AChTour::BeginPlay()
{
	Super::BeginPlay();
}

void AChTour::ConfigurePawn()
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
