// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MappingAlgo.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLFQUEST_API UMappingAlgo : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMappingAlgo();
	bool doIntersect(FVector p1, FVector q1, FVector p2, FVector q2);
	int orientation(FVector p, FVector q, FVector r);
	bool onSegment(FVector p, FVector q, FVector r);
	bool isInside(TArray<FVector> polygon, int32 n, FVector p);
	bool isSquareInside(TArray<FVector> c, FVector p);
	UFUNCTION(BlueprintCallable)
		TArray<FVector> GeneratePoints(TArray<FVector> Outline);
public:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FVector> MapOutline;
	//Tile set ups
	
	int tileSize = 40; //10 by 10 pixels
	TArray<FVector> newCoordinates;
	//Bounds
	int highestX = 0;
	int highestY = 0;
	int lowestX = 99999;
	int lowestY = 99999;
	// Define Infinite (Using INT_MAX caused overflow problems)
	int inf = 10000;
};
