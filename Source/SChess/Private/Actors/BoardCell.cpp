// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BoardCell.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABoardCell::ABoardCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshConponent"));
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
	}

	//const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Engine/BasicShapes/Plane.Plane")); //
	//if (MeshObj.Succeeded() && MeshComponent)
	//{
	//	StaticMesh = MeshObj.Object;
	//	MeshComponent->SetStaticMesh(StaticMesh);
	//}

}

void ABoardCell::SetActorMesh(UStaticMesh* Mesh)
{
	StaticMesh = Mesh;



	if (StaticMesh && MeshComponent)
	{
		MeshComponent->SetStaticMesh(StaticMesh);
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

void ABoardCell::GetIndex(int32& IndX, int32& IndY)
{
	IndX = IndexX;
	IndY = IndexY;
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

