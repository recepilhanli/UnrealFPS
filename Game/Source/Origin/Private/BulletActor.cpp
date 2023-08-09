// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletActor.h"
#include "Components/CapsuleComponent.h"
#include "AI_Interface.h"
#include "Components/AudioComponent.h"
#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"

ABulletActor::ABulletActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	// declare overlap events
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::OnOverlapBegin);

	SetLifeSpan(5.0f);
}

void ABulletActor::OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

	USoundCue *cue = SoundManager::GetInstance()->GetSound(SOUND_BULLET_HIT);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),cue,OtherActor->GetActorLocation());
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		IAI_Interface *ai = Cast<IAI_Interface>(OtherActor);
		SetActorRotation(FRotator(0, 0, 180));
		if (ai)
			ai->GiveHealth(-10, GetActorLocation());
	}

	Destroy();
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
