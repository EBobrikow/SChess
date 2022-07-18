// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
//#include "Actors/Pawns/BasePawn.h"
#include "BoardCell.generated.h"

UCLASS()
class SCHESS_API ABoardCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardCell();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* StaticMesh;

	UFUNCTION(BlueprintCallable)
	void SetActorMesh(UStaticMesh* Mesh);

	UFUNCTION(BlueprintCallable)
	void SetActorMeshMaterial(UMaterial* Material);

	UFUNCTION(BlueprintCallable)
	void SetIndex(int32 IndX, int32 IndY);

	UFUNCTION(BlueprintCallable)
	void GetIndex(int32& IndX, int32& IndY) const;

	UFUNCTION(BlueprintCallable)
	void SetPawnOnCell(ABasePawn* ChessPawn);

	UFUNCTION(BlueprintCallable)
	ABasePawn* GetPawnOnCell() const;

	UPROPERTY(ReplicatedUsing = OnRep_IsHighlighted)
	bool bIsHighlighted;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private: 

	UFUNCTION()
	void OnRep_IsHighlighted();

	UPROPERTY(VisibleAnywhere, Replicated)
	int32 IndexX;

	UPROPERTY(VisibleAnywhere, Replicated)
	int32 IndexY;

	UPROPERTY(Replicated)
	ABasePawn* PawnOnCell = nullptr;

	

};
