// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Anim_Footstep.generated.h"

/**
 * 
 */
UCLASS()
class ORIGIN_API UAnim_Footstep : public UAnimNotify
{
	GENERATED_BODY()
	
	private:
	UAnim_Footstep();
	public:
	void Recieved_NotifyStart();

};
