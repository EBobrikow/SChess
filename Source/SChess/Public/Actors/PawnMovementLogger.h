// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Globals.h"
#include "PawnMovementLogger.generated.h"

UCLASS()
class SCHESS_API APawnMovementLogger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APawnMovementLogger();

	UFUNCTION(BlueprintCallable)
	void LogPawnMovement(FPawnMovementInfo MovementInfo);

	UFUNCTION(BlueprintCallable)
	FPawnMovementInfo GetLastMovementInfo() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private: 

	FString MovementLogToString(FPawnMovementInfo MovInfo) const;

	FString PawnColorToString(TEnumAsByte<PawnColorType> Color) const;

	FString PawnTypeToString(TEnumAsByte<PawnTypes> Type) const;

	FString IndXtoString(int32 IndexX) const;

	UPROPERTY()
	TArray<FPawnMovementInfo> MovementLogList;
};
