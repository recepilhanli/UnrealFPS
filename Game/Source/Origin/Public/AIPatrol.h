// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIPatrol.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORIGIN_API UAIPatrol : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIPatrol();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol Settings")
	class USplineComponent* PatrolLine;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Patrol Settings")
	int CurrentPosition;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
