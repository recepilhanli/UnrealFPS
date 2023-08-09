// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimMontageManager.h"

AnimMontageManager::AnimMontageManager()
{
    LoadAllMontages();
}

AnimMontageManager * AnimMontageManager::Instance = nullptr;





AnimMontageManager * AnimMontageManager::GetInstance()
{
if(Instance == nullptr) Instance = new AnimMontageManager();
return Instance;
}

UAnimMontage * AnimMontageManager::GetAnimMontage(FString name)
{
    UAnimMontage * montage = Montages.FindRef(*name);
    return montage;
}


void AnimMontageManager::LoadAllMontages()
{
LoadMontage(MONTAGE_FIRE);
LoadMontage(MONTAGE_HIT1);
LoadMontage(MONTAGE_HIT2);
LoadMontage(MONTAGE_HIT3);
LoadMontage(MONTAGE_HIT4);
}


void AnimMontageManager::LoadMontage(FString name)
{

    FString path = TEXT("/Game/Montages/") + name;

    UAnimMontage *montage = LoadObject<UAnimMontage>(nullptr, *path);

    if (montage != nullptr)
        Montages.Add(name, montage);
}


