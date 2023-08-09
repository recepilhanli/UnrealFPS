#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#define SOUND_PLAYER_TAKEDAMAGE 		TEXT("player/pain")
#define SOUND_PLAYER_JUMP 				TEXT("player/jump")
#define SOUND_PLAYER_STEP_SAND 			TEXT("player/step_sand")
#define SOUND_WEAPON_AR_FIRE			TEXT("weapon/ar4_fire")
#define SOUND_BULLET_HIT    			TEXT("weapon/bullet_hit")

#define PLAY_SOUND(audiocomponent, sound) do { \
    SoundManager* soundmanagerInstance = SoundManager::GetInstance(); \
    audiocomponent->SetSound(soundmanagerInstance->GetSound(sound)); \
    audiocomponent->Play(); \
} while(0)


#define PLAY_SOUND_2D(Sound) \
    { \
        SoundManager* soundmanagerInstance = SoundManager::GetInstance(); \
        USoundBase* soundToPlay = soundmanagerInstance->GetSound(Sound); \
        if (soundToPlay) \
        { \
            UGameplayStatics::PlaySound2D(GetWorld(), soundToPlay); \
        } \
    }



class ORIGIN_API SoundManager
{

public:

static SoundManager * GetInstance();
USoundCue * GetSound(FString name);
void Clear();


private:
	SoundManager();
	~SoundManager();	
	static SoundManager* Instance;
	TMap<FString, USoundCue*> SoundMap;
	void LoadAllSounds();
	void LoadSound(FString name);
};

