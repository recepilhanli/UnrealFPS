// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
/**
 * 
 */


#define PLAY_MONTAGE(SkeletalMeshComponent, Name) \
{ \
	AnimMontageManager * instance = AnimMontageManager::GetInstance(); \
    UAnimMontage* Montage = instance->GetAnimMontage(Name); \
    if (Montage) \
    { \
        UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance(); \
        if (AnimInstance) \
        { \
            AnimInstance->Montage_Play(Montage); \
        } \
    } \
}

#define MONTAGE_FIRE (TEXT("FireMontage"))
#define MONTAGE_HIT1 (TEXT("HitReact1"))
#define MONTAGE_HIT2 (TEXT("HitReact2"))
#define MONTAGE_HIT3 (TEXT("HitReact3"))
#define MONTAGE_HIT4 (TEXT("HitReact4"))

class ORIGIN_API AnimMontageManager
{

public:
static AnimMontageManager* GetInstance();
UAnimMontage * GetAnimMontage(FString name);


private:
	AnimMontageManager();
	~AnimMontageManager();
	static AnimMontageManager * Instance;
	TMap<FString,UAnimMontage *> Montages;
	void LoadMontage(FString nme);
	void LoadAllMontages();
};
