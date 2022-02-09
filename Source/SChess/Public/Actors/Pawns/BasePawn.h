// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Globals.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "BasePawn.generated.h"

UCLASS()
class SCHESS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();



	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<PawnTypes> PawnType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* PawnMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInstance* PawnMeshWhiteMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInstance* PawnMeshBlackMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<PawnColorType> PawnColor;

	virtual void InitFigure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
	virtual void ConfigurePawn();

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	

};
