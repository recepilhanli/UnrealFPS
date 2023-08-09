// Fill out your copyright notice in the Description page of Project Settings.

#include "Anim_Footstep.h"
#include "GameFramework/Character.h"

UAnim_Footstep::UAnim_Footstep()
{

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, TEXT("Notify"));
    ACharacter *character = Cast<ACharacter>(GetContainingAsset());

    if (character != nullptr && GEngine != nullptr)
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, character->GetActorNameOrLabel());
}



void UAnim_Footstep::Recieved_NotifyStart()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Notification!");
}
