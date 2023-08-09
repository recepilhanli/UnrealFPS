// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundManager.h"
#include "AI_SoldierCloud.h"

SoundManager::SoundManager()
{
    LoadAllSounds();
}
SoundManager::~SoundManager()
{
}

SoundManager *SoundManager::Instance = nullptr;

SoundManager *SoundManager::GetInstance()
{
    if (Instance == nullptr)
    {
        Instance = new SoundManager();
    }

    return Instance;
}

USoundCue *SoundManager::GetSound(FString name)
{
    USoundCue *sound = SoundMap.FindRef(*name);
    if (!sound)
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Couldn't find spcified sound!"));
    return sound;
}

void SoundManager::LoadAllSounds()
{
    LoadSound(SOUND_PLAYER_TAKEDAMAGE);
    LoadSound(SOUND_PLAYER_JUMP);
    LoadSound(SOUND_PLAYER_STEP_SAND);
    LoadSound(SOUND_WEAPON_AR_FIRE);   
    LoadSound(SOUND_BULLET_HIT);   
}

void SoundManager::LoadSound(FString name)
{
    FString path = TEXT("/Game/Sounds/") + name;

    USoundCue *Sound = LoadObject<USoundCue>(nullptr, *path);

    if (Sound != nullptr)
        SoundMap.Add(name, Sound);
}

void SoundManager::Clear()
{
    delete Instance;
    Instance = nullptr;
}