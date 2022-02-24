// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BoardCell.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABoardCell::ABoardCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshConponent"));
	if (MeshComponent)
	{
		RootComponent = MeshComponent;
		//MeshComponent->SetupAttachment(RootComponent);
		MeshComponent->SetIsReplicated(true);
	}

	//const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Plane.Plane")); //
	//if (MeshObj.Succeeded() && MeshComponent)
	//{
	//	StaticMesh = MeshObj.Object;
	//	MeshComponent->SetStaticMesh(StaticMesh);
	//}

	bReplicates = true;
	

}

void ABoardCell::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoardCell, PawnOnCell);
}

void ABoardCell::SetActorMesh(UStaticMesh* Mesh)
{
	StaticMesh = Mesh;



	if (StaticMesh && MeshComponent)
	{
		MeshComponent->SetStaticMesh(StaticMesh);
		MeshComponent->BodyInstance.SetCollisionProfileName("WorldDynamic");
	}
}

void ABoardCell::SetActorMeshMaterial(UMaterial* Material)
{
	if (Material && StaticMesh && MeshComponent)
	{
		
		StaticMesh->SetMaterial(0, Material);
		MeshComponent->SetStaticMesh(StaticMesh);
	}
		

}

void ABoardCell::SetIndex(int32 IndX, int32 IndY)
{
	IndexX = IndX;
	IndexY = IndY;
}

void ABoardCell::GetIndex(int32& IndX, int32& IndY) const
{
	IndX = IndexX;
	IndY = IndexY;
}

void ABoardCell::SetPawnOnCell(ABasePawn* ChessPawn)
{
	PawnOnCell = ChessPawn;
	
}

ABasePawn* ABoardCell::GetPawnOnCell() const
{
	if (PawnOnCell)
		return PawnOnCell;
	else
		return nullptr;
}

// Called when the game starts or when spawned
void ABoardCell::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ABoardCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

