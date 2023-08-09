// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AI_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAI_Interface : public UInterface
{
	GENERATED_BODY()
};


#define SOUNDTYPE_FOOTSTEP (1) 



/**
 * 
 */
class ORIGIN_API IAI_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
    virtual void MovePos(FVector * ToPosition) = 0;
	virtual void GiveHealth(float h, FVector from = FVector::ZeroVector) = 0;
	virtual void Kill() = 0;	
	virtual void MakeSound(int soundtype = 0) = 0;
};
