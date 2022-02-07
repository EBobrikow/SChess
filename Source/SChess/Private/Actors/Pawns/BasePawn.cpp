// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pawns/BasePawn.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent); 
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePawn::InitFigure()
{
	ISMComp = NewObject<UInstancedStaticMeshComponent>(this);
	ISMComp->RegisterComponent();
	if (PawnMesh)
	{
		if (PawnColor == PawnColorType::Black && PawnMeshBlackMaterial)
			PawnMesh->SetMaterial(0, PawnMeshBlackMaterial);
		else if(PawnColor == PawnColorType::White && PawnMeshWhiteMaterial)
			PawnMesh->SetMaterial(0, PawnMeshWhiteMaterial);

		/*if (MeshComponent)
		{
			MeshComponent->SetStaticMesh(PawnMesh);
		}*/

		FVector Scale(27.0f,4.5f,27.0f);
		//MeshComponent->SetRelativeScale3D(Scale);
		ISMComp->SetStaticMesh(PawnMesh);
		ISMComp->SetRelativeScale3D(Scale);
	}
	
	ISMComp->SetFlags(RF_Transactional);
	this->AddInstanceComponent(ISMComp);
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

