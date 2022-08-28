// Fill out your copyright notice in the Description page of Project Settings.



#include "Actors/Pawns/BasePawn.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	if (MeshComponent)
	{
		RootComponent = MeshComponent;
		MeshComponent->SetIsReplicated(true);
	}
	
	//MeshComponent->SetupAttachment(RootComponent); 
	bReplicates = true;
}

void ABasePawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePawn, PawnColor);
	DOREPLIFETIME(ABasePawn, PawnType);
	DOREPLIFETIME(ABasePawn, PawnMeshWhiteMaterial);
	DOREPLIFETIME(ABasePawn, PawnMeshBlackMaterial);
	DOREPLIFETIME(ABasePawn, Foothold);
	DOREPLIFETIME(ABasePawn, bIsFirstMove);
	DOREPLIFETIME(ABasePawn, DynamicMat);
	DOREPLIFETIME(ABasePawn, MeshComponent);
	DOREPLIFETIME(ABasePawn, PawnMesh);
}

UStaticMeshComponent* ABasePawn::GetMeshComponent() const
{
	return MeshComponent;
}

TArray<ABoardCell*> ABasePawn::GetCellsBetweenPawns(ABasePawn* OtherPawn)
{

	TArray<ABoardCell*> Output = TArray<ABoardCell*>();
	int32 FPawnX = 0, FPawnY = 0, SPawnX = 0, SPawnY = 0;
	if (PawnType == PawnTypes::Horse || OtherPawn->PawnType == PawnTypes::Horse
		|| PawnType == PawnTypes::Pawn || OtherPawn->PawnType == PawnTypes::Pawn) // there ara no cells between pawn ang horse atack tragectory
		return Output;

	GetFoohold()->GetIndex(FPawnX, FPawnY);
	OtherPawn->GetFoohold()->GetIndex(SPawnX, SPawnY);

	ASChessGameModeBase* GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (FPawnX == SPawnX) //Straight-vertical
	{

		int32 start = 0, end = 0;
		(FPawnY > SPawnY) ? (start = SPawnY) : (start = FPawnY);
		(FPawnY > SPawnY) ? (end = FPawnY) : (end = SPawnY);

		for (int32 Y = start; Y < end; Y++)
		{
			Output.Add(GameMode->GetCellByIndex(FPawnX, Y));
		}


	}
	else if (FPawnY == SPawnY) //Straight-horizontal
	{
		int32 start = 0, end = 0;
		(FPawnX > SPawnX) ? (start = SPawnX) : (start = FPawnX);
		(FPawnX > SPawnX) ? (end = FPawnX) : (end = SPawnX);

		for (int32 X = start; X < end; X++)
		{
			Output.Add(GameMode->GetCellByIndex(X, FPawnY));
		}
	}
	else if ((FPawnX - SPawnX) == (FPawnY - SPawnY)) // cross-right
	{
		int32 startX = 0, startY = 0, difference = 0;
		if (FPawnY > SPawnY)
		{
			difference = FPawnY - SPawnY;
			startX = SPawnX;
			startY = SPawnY;
		}
		else
		{
			difference = SPawnY - FPawnY;
			startX = FPawnX;
			startY = FPawnY;
		}

		for (int32 i = 1; i < difference; i++)
		{
			Output.Add(GameMode->GetCellByIndex(startX + i, startY + i));
		}

	}
	else // cross-left
	{
		int32 startX = 0, startY = 0, difference = 0;


		if (FPawnX > SPawnX)
		{
			difference = FPawnX - SPawnX;
			startX = SPawnX;
			startY = SPawnY;
		}
		else
		{
			difference = SPawnX - FPawnX;
			startX = FPawnX;
			startY = FPawnY;
		}

		for (int32 i = 1; i < difference; i++)
		{
			Output.Add(GameMode->GetCellByIndex(startX + i, startY - i));
		}

	}



	return Output;
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePawn::SetColor_Implementation()
{
	if (PawnColor == PawnColorType::Black && PawnMeshBlackMaterial)
	{
		//DynamicMat = UMaterialInstanceDynamic::Create(PawnMeshBlackMaterial, this);
		this->MeshComponent->SetMaterial(0, PawnMeshBlackMaterial);
		MeshComponent->SetIsReplicated(true);
	}
	else if (PawnColor == PawnColorType::White && PawnMeshWhiteMaterial)
	{
		//DynamicMat = UMaterialInstanceDynamic::Create(PawnMeshWhiteMaterial, this);
		this->MeshComponent->SetMaterial(0, PawnMeshWhiteMaterial);
		MeshComponent->SetIsReplicated(true);
	}
		
}

void ABasePawn::ConfigurePawn()
{
	
}

TArray<ABoardCell*> ABasePawn::GetPossibleSteps(bool IgnorePawnForwardMov)
{
	int32 CurX = 0, CurY = 0;
	ABoardCell* Cell = GetFoohold();
	TArray<ABoardCell*> OutArray;
	bool DetectedPawnSameColor = false;
	int32 moveDirectionX = 0, moveDirectionY = 0;
	bool empty = false;

	moveDirectionX = moveDirectionY = (PawnColor == PawnColorType::White) ? 1 : -1;

	ASChessGameModeBase* GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Cell->GetIndex(CurX, CurY);
	if (GameMode)
	{
		for (auto Direction : MovementPattern)
		{
			switch (Direction)
			{
			case MovementDirections::Forward:

				if (MovementRange == MovementRange::OneStep)
				{
					if (!(IgnorePawnForwardMov && PawnType == PawnTypes::Pawn))
					{


						if (CheckIsAvailableStep(CurX, CurY + moveDirectionY, empty))
						{
							OutArray.Add(GameMode->GetCellByIndex(CurX, CurY + moveDirectionY));
						}
						if (bIsFirstMove && PawnType == PawnTypes::Pawn)
						{
							if (CheckIsAvailableStep(CurX, CurY + (moveDirectionY * 2), empty))
							{
								OutArray.Add(GameMode->GetCellByIndex(CurX, CurY + (moveDirectionY * 2)));
							}
						}
					}
				}
				else
				{
					int32 step = 1;
					bool isGoFurther = true;
					while (CheckIsAvailableStep(CurX, CurY + (moveDirectionY * step), isGoFurther))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX, CurY + (moveDirectionY * step)));
						step++;
						if (!isGoFurther)
						{
							break;
						}
					}
				}

				

				break;

			case MovementDirections::Forward_Left:
				if (MovementRange == MovementRange::OneStep)
				{
					if (PawnType == PawnTypes::Pawn)
					{
						if (CheckStepBorders(CurX - moveDirectionX) && CheckStepBorders(CurY + moveDirectionY))
						{
							if (GameMode->IsPawnExistOnCell(CurX - moveDirectionX, CurY + moveDirectionY, PawnColor, DetectedPawnSameColor))
							{
								if (!DetectedPawnSameColor)
								{
									OutArray.Add(GameMode->GetCellByIndex(CurX - moveDirectionX, CurY + moveDirectionY));
								}

							}
						}
					}
					else
					{
						
						if (CheckIsAvailableStep(CurX - moveDirectionX, CurY + moveDirectionY, empty))
						{
							OutArray.Add(GameMode->GetCellByIndex(CurX - moveDirectionX, CurY + moveDirectionY));
						}
					}
				}
				else
				{
					int32 step = 1;
					bool isGoFurther = true;
					while (CheckIsAvailableStep(CurX - (moveDirectionX * step), CurY + (moveDirectionY * step), isGoFurther))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX - (moveDirectionX * step), CurY + (moveDirectionY * step)));
						step++;
						if (!isGoFurther)
						{
							break;
						}
					}
				}

				break;

			case MovementDirections::Forward_Right:
				if (MovementRange == MovementRange::OneStep)
				{
					if (PawnType == PawnTypes::Pawn)
					{
						if (CheckStepBorders(CurX + moveDirectionX) && CheckStepBorders(CurY + moveDirectionY))
						{
							if (GameMode->IsPawnExistOnCell(CurX + moveDirectionX, CurY + moveDirectionY, PawnColor, DetectedPawnSameColor))
							{
								if (!DetectedPawnSameColor)
								{
									OutArray.Add(GameMode->GetCellByIndex(CurX + moveDirectionX, CurY + moveDirectionY));
								}

							}
						}
					}
					else
					{
						if (CheckIsAvailableStep(CurX + moveDirectionX, CurY + moveDirectionY, empty))
						{
							OutArray.Add(GameMode->GetCellByIndex(CurX + moveDirectionX, CurY + moveDirectionY));
						}
					}
				}
				else
				{
					int32 step = 1;
					bool isGoFurther = true;
					while (CheckIsAvailableStep(CurX + (moveDirectionX * step), CurY + (moveDirectionY * step), isGoFurther))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX + (moveDirectionX * step), CurY + (moveDirectionY * step)));
						step++;
						if (!isGoFurther)
						{
							break;
						}
					}
				}

				break;

			case MovementDirections::Left:
				if (MovementRange == MovementRange::OneStep)
				{
					if (CheckIsAvailableStep(CurX - moveDirectionX, CurY, empty))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX - moveDirectionX, CurY));
					}
				}
				else
				{
					int32 step = 1;
					bool isGoFurther = true;
					while (CheckIsAvailableStep(CurX - (moveDirectionX * step), CurY, isGoFurther))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX - (moveDirectionX * step), CurY));
						step++;
						if (!isGoFurther)
						{
							break;
						}
					}
				}
				break;

			case MovementDirections::Back_Left:
				if (MovementRange == MovementRange::OneStep)
				{
					if (CheckIsAvailableStep(CurX - moveDirectionX, CurY - moveDirectionY, empty))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX - moveDirectionX, CurY - moveDirectionY));
					}
					
				}
				else
				{
					int32 step = 1;
					bool isGoFurther = true;
					while (CheckIsAvailableStep(CurX - (moveDirectionX * step), CurY - (moveDirectionY * step), isGoFurther))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX - (moveDirectionX * step), CurY - (moveDirectionY * step)));
						step++;
						if (!isGoFurther)
						{
							break;
						}
					}
				}
				break;

			case MovementDirections::Back:
				if (MovementRange == MovementRange::OneStep)
				{
					if (CheckIsAvailableStep(CurX, CurY - moveDirectionY, empty))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX, CurY - moveDirectionY));
					}
					
				}
				else
				{
					int32 step = 1;
					bool isGoFurther = true;
					while (CheckIsAvailableStep(CurX, CurY - (moveDirectionY * step), isGoFurther))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX, CurY - (moveDirectionY * step)));
						step++;
						if (!isGoFurther)
						{
							break;
						}
					}
				}
				break;

			case MovementDirections::Back_Right:
				if (MovementRange == MovementRange::OneStep)
				{
					if (CheckIsAvailableStep(CurX + moveDirectionX, CurY - moveDirectionY, empty))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX + moveDirectionX, CurY - moveDirectionY));
					}
					
				}
				else
				{
					int32 step = 1;
					bool isGoFurther = true;
					while (CheckIsAvailableStep(CurX + (moveDirectionX * step), CurY - (moveDirectionY * step), isGoFurther))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX + (moveDirectionX * step), CurY - (moveDirectionY * step)));
						step++;
						if (!isGoFurther)
						{
							break;
						}
					}
				}
				break;

			case MovementDirections::Right:
				if (MovementRange == MovementRange::OneStep)
				{
					if (CheckIsAvailableStep(CurX + moveDirectionX, CurY, empty))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX + moveDirectionX, CurY));
					}
				}
				else
				{
					int32 step = 1;
					bool isGoFurther = true;
					while (CheckIsAvailableStep(CurX + (moveDirectionX * step), CurY, isGoFurther))
					{
						OutArray.Add(GameMode->GetCellByIndex(CurX + (moveDirectionX * step), CurY));
						step++;
						if (!isGoFurther)
						{
							break;
						}
					}
				}
				break;

			case MovementDirections::T_Forward_Right:
				
				if (CheckIsAvailableStep(CurX + 1, CurY +2, empty))
				{
					OutArray.Add(GameMode->GetCellByIndex(CurX + 1, CurY + 2));
				}
				break;

			case MovementDirections::T_Forward_Left:
			
				if (CheckIsAvailableStep(CurX - 1, CurY + 2, empty))
				{
					OutArray.Add(GameMode->GetCellByIndex(CurX - 1, CurY + 2));
				}
				break;

			case MovementDirections::T_Left_Right:
			
				if (CheckIsAvailableStep(CurX - 2, CurY + 1, empty))
				{
					OutArray.Add(GameMode->GetCellByIndex(CurX - 2, CurY + 1));
				}
				break;

			case MovementDirections::T_Left_Left:
				
				if (CheckIsAvailableStep(CurX - 2, CurY - 1, empty))
				{
					OutArray.Add(GameMode->GetCellByIndex(CurX - 2, CurY - 1));
				}
				break;

			case MovementDirections::T_Back_Right:
			
				if (CheckIsAvailableStep(CurX + 1, CurY - 2, empty))
				{
					OutArray.Add(GameMode->GetCellByIndex(CurX + 1, CurY - 2));
				}
				break;

			case MovementDirections::T_Back_Left:
				
				if (CheckIsAvailableStep(CurX - 1, CurY - 2, empty))
				{
					OutArray.Add(GameMode->GetCellByIndex(CurX - 1, CurY - 2));
				}
				break;

			case MovementDirections::T_Right_Right:
				
				if (CheckIsAvailableStep(CurX + 2, CurY - 1, empty))
				{
					OutArray.Add(GameMode->GetCellByIndex(CurX + 2, CurY - 1));
				}
				break;

			case MovementDirections::T_Right_Left:
				
				if (CheckIsAvailableStep(CurX + 2, CurY + 1, empty))
				{
					OutArray.Add(GameMode->GetCellByIndex(CurX + 2, CurY + 1));
				}
				break;

			default:
				break;
			}
		}
	}

	

	return OutArray;
}


ABoardCell* ABasePawn::GetFoohold() const
{
	return Foothold;
}

void ABasePawn::SetFoothold(ABoardCell* footholdCell)
{
	Foothold = footholdCell;
}

void ABasePawn::OnRep_PawnColor()
{
	SetColor();
}



void ABasePawn::InitFigure_Implementation()
{
	
	if (PawnMesh)
	{

		if (MeshComponent)
		{
			MeshComponent->SetStaticMesh(PawnMesh);
			MeshComponent->BodyInstance.SetCollisionProfileName("Pawn");
		}
		const ENetMode NetMode = GetNetMode();
		if (NetMode == NM_ListenServer)
		{
			OnRep_PawnColor();
		}
		
	}
	
	ConfigurePawn();
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

bool ABasePawn::CheckStepBorders(int32 Pos)
{
	return (Pos <= 8 && Pos > 0);
}

bool ABasePawn::CheckIsAvailableStep(int32 X, int32 Y, bool& CanGoFurther)
{
	bool DetectedPawnSameColor = false;
	ASChessGameModeBase* GameMode = Cast<ASChessGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CheckStepBorders(Y) && CheckStepBorders(X)) 
	{
		if (!GameMode->IsPawnExistOnCell(X, Y, PawnColor, DetectedPawnSameColor))
		{
			return true;
		} 
		else if (PawnType != PawnTypes::Pawn && !DetectedPawnSameColor)
		{
			CanGoFurther = false;
			return true;
				
		}
	}
	return false;
}

