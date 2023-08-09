// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "FootStepNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ORIGIN_API UFootStepNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;

};
