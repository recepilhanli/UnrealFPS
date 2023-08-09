// Fill out your copyright notice in the Description page of Project Settings.

#include "FootStepNotifyState.h"
#include "GameFramework/Character.h"
#include "AI_Interface.h"

void UFootStepNotifyState::NotifyBegin(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, float TotalDuration)
{

    IAI_Interface *character = Cast<IAI_Interface>(MeshComp->GetOwner());
    if (character != nullptr) character->MakeSound(SOUNDTYPE_FOOTSTEP);
}
