// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI_Interface.h"
#include "Soldier.generated.h"


UCLASS()
class ORIGIN_API ASoldier : public ACharacter, public IAI_Interface
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ASoldier();

	UPROPERTY(BlueprintReadWrite, Category = "AI Stuffs")
	class UPawnSensingComponent* Sense;

	UPROPERTY(BlueprintReadWrite, Category = "AI Stuffs")
	float Health = 100;
	UPROPERTY(BlueprintReadWrite, Category = "AI Stuffs")
	TSubclassOf<class ABulletActor> BulletRef;
	
	
    UFUNCTION()
    void OnSeePawn(APawn* SeenPawn);
	void Fire();
	void Warn();

	UPROPERTY(BlueprintReadOnly, Category = "AI Stuffs")
	APawn* Seen;

	float LastSeen;

	bool IsMoving;
	FVector MovingPos;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float LookY = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int WeaponID = 0;

	float WeaponRate = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool InDanger = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SoldierType = 0;	 //0: Agressive, 1: Clever, 2: Coward

	class UAIPatrol * Patrol;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *WeaponMesh;

	void Roaming(FVector Location = FVector::Zero(),float range = 1000.0f);
	void OnReachDestination();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void MovePos(FVector * ToPosition) override;
	virtual void GiveHealth(float h, FVector from = FVector::ZeroVector) override;
	virtual void Kill() override;
	virtual void MakeSound(int soundtype) override;


};
