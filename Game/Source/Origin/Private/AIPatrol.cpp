// Fill out your copyright notice in the Description page of Project Settings.

#include "AIPatrol.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
UAIPatrol::UAIPatrol()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	
}

void UAIPatrol::BeginPlay()
{
Super::BeginPlay();

PatrolLine = GetOwner()->FindComponentByClass<USplineComponent>();

if(PatrolLine == NULL)
{
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red,TEXT("Couldn't find patrol spline!"));
}


}