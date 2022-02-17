// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Globals.h"
#include "Kismet/GameplayStatics.h"
#include "SChessGameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "BasePawn.generated.h"

class ABoardCell;
//class ASChessGameModeBase;

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

	virtual void ConfigurePawn();

	virtual TArray<ABoardCell*> GetPossibleSteps(bool IgnorePawnForwardMov = false);

	UFUNCTION(BlueprintCallable)
	ABoardCell* GetFoohold() const;

	UFUNCTION(BlueprintCallable)
	void SetFoothold(ABoardCell* footholdCell);


	UPROPERTY()
	bool bIsFirstMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY()
	TSet<MovementDirections> MovementPattern;

	UPROPERTY()
	TEnumAsByte<MovementRange> MovementRange;
	
	UPROPERTY()
	ABoardCell* Foothold;

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UFUNCTION()
	bool CheckStepBorders(int32 Pos);

	UFUNCTION()
	bool CheckIsAvailableStep(int32 X, int32 Y, bool& CanGoFurther);

};
