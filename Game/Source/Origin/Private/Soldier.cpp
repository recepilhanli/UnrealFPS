// Fill out your copyright notice in the Description page of Project Settings.

#include "Soldier.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SplineComponent.h"
#include "AIPatrol.h"
#include "Weapons.h"
#include "BulletActor.h"
#include "Components/CapsuleComponent.h"
#include "AI_SoldierCloud.h"
#include "NavigationSystem.h"
#include "AnimMontageManager.h"

#define RUN_SPEED (300)
#define WALK_SPEED (100)

// Sets default values
ASoldier::ASoldier()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sense"));

	Sense->OnSeePawn.AddDynamic(this, &ASoldier::OnSeePawn);
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetMesh(), "hand_l");
	Weapons *Wepinstance = Weapons::GetInstance();
	WeaponMesh->SetStaticMesh(Wepinstance->GetWeapon(WeaponID)->WeaponMesh);

	GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
}

void ASoldier::MovePos(FVector *ToPosition)
{
	if (!Seen)
	{
		IsMoving = true;
		MovingPos = *ToPosition;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), *ToPosition);
}

void ASoldier::GiveHealth(float h, FVector from)
{
	Health += h;

	if (h < 0)
	{
		int random = FMath::RandRange(0, 4);

		if (random == 0)
			PLAY_MONTAGE(GetMesh(), MONTAGE_HIT1);
		if (random == 1)
			PLAY_MONTAGE(GetMesh(), MONTAGE_HIT2);
		if (random == 2)
			PLAY_MONTAGE(GetMesh(), MONTAGE_HIT3);
		if (random == 3)
			PLAY_MONTAGE(GetMesh(), MONTAGE_HIT4);
	}

	if (h < 0 && Seen == nullptr)
	{

		Warn();
	}

	if (from != FVector::ZeroVector && Seen == nullptr)
	{
		FVector ActorLocation = GetActorLocation();
		FRotator NewRotation = FRotationMatrix::MakeFromX(from - ActorLocation).Rotator();
		NewRotation.Pitch = 0;
		SetActorRotation(NewRotation);
	}

	if (Health <= 0)
		Kill();
}

void ASoldier::Kill()
{
	AI_SoldierCloud *cloud = AI_SoldierCloud::GetInstance();
	cloud->Soldiers.Remove(this);
	USkeletalMeshComponent *MeshComponent = GetMesh();
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetAllBodiesSimulatePhysics(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComponent->WakeAllRigidBodies();
	if (Patrol && Patrol->PatrolLine)
		Patrol->PatrolLine->DestroyComponent();
	if (Patrol)
		Patrol->DestroyComponent();
	MeshComponent->Stop();
	if (Seen)
		MeshComponent->AddImpulse(-GetActorForwardVector() * 100, NAME_None, true);
	Sense->DestroyComponent();
	WeaponMesh->DestroyComponent();
	Seen = nullptr;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASoldier::Roaming(FVector Location, float range)
{

	UNavigationSystemV1 *NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{

		FVector Origin = Location;
		if (Origin == FVector::Zero())
		{
			if (InDanger && GetWorld()->TimeSeconds > 15)
			{
				AI_SoldierCloud *cloud = AI_SoldierCloud::GetInstance();
				ACharacter *player = cloud->Player;

				if (player == nullptr)
					Origin = GetActorLocation();
				else
					Origin = player->GetActorLocation();
			}
			else
				Origin = GetActorLocation();
		}

		FNavLocation RandomLocation;
		NavSystem->GetRandomPointInNavigableRadius(Origin, range, RandomLocation);
		FVector LocVector = RandomLocation;
		MovePos(&LocVector);
	}
}

void ASoldier::OnReachDestination()
{
	IsMoving = false;
	MovingPos = FVector::ZeroVector;
	if (InDanger)
		Roaming();
}

void ASoldier::Fire()
{
	float currentTime = GetWorld()->TimeSeconds*1000;
	if(WeaponRate >  currentTime) return;

	// PLAY_MONTAGE(GetMesh(), MONTAGE_FIRE);
	Weapons *wepinstance = Weapons::GetInstance();

	if (!Seen || !wepinstance->BulletClass)
		return;
	FVector BulletStartPos = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
	ABulletActor *bullet = GetWorld()->SpawnActor<ABulletActor>(wepinstance->BulletClass, BulletStartPos, GetActorRotation());

	if (bullet)
	{
		FVector Direction = (Seen->GetActorLocation() - GetActorLocation());
		bullet->ProjectileMovement->Velocity = Direction * bullet->ProjectileMovement->InitialSpeed;
		Direction.Normalize();
	}

	WeaponRate = currentTime + wepinstance->GetWeapon(WeaponID)->Rate;
}

// Called when the game starts or when spawned
void ASoldier::BeginPlay()
{

	Super::BeginPlay();

	AI_SoldierCloud *cloud = AI_SoldierCloud::GetInstance();
	cloud->Soldiers.Add(this);

	if (InDanger)
	{
		Roaming();
		return;
	}

	Patrol = FindComponentByClass<UAIPatrol>();

	if (Patrol)
	{

		int32 NumPoints = Patrol->PatrolLine->GetNumberOfSplinePoints();

		for (int32 i = 0; i < NumPoints; i++)
		{
			FVector PointLocation = Patrol->PatrolLine->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
			MovePos(&PointLocation);
			UE_LOG(LogTemp, Warning, TEXT("Spline Point %d: (%f, %f, %f)"), i, PointLocation.X, PointLocation.Y, PointLocation.Z);
		}
	}
	//	Tags.AddTag(FName("MyCustomTag"));
}

void ASoldier::OnSeePawn(APawn *SeenPawn)
{

	if (SeenPawn->ActorHasTag(TEXT("Enemy Soldier")))
		return;

	LastSeen = GetWorld()->TimeSeconds + 5;
	IsMoving = false;
	Seen = SeenPawn;
	Warn();

	GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;
}

void ASoldier::Warn()
{
	InDanger = true;

	AI_SoldierCloud *cloud = AI_SoldierCloud::GetInstance();
	cloud->Alarm(this, 4000);
}

void ASoldier::MakeSound(int soundtype)
{
}

// Called every frame
void ASoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Seen && IsMoving && MovingPos != FVector::Zero())
	{

		float dist = FVector::Distance(GetActorLocation(), MovingPos);
		if (dist < 120)
			OnReachDestination();
	}

	if (Seen != NULL)
	{

		if (LastSeen < (GetWorld()->TimeSeconds))
		{
			Seen = NULL;
			GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
			return;
		}

		FVector Destination = Seen->GetActorLocation();

		float dist = FVector::Distance(GetActorLocation(), Destination);

		if ((dist < 1250 && FMath::RandRange(0, 5) == 1) || dist < 800)
		{
			GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
			FVector ActorLocation = GetActorLocation();
			FRotator NewRotation = FRotationMatrix::MakeFromX(Destination - ActorLocation).Rotator();

			float sopitch = GetMesh()->GetSocketRotation(TEXT("spine_02")).Pitch;
			float nPitch = NewRotation.Pitch;

			LookY = nPitch - sopitch + 75;

			NewRotation.Pitch = 0;

			SetActorRotation(NewRotation);

			Fire();
			GetController()->StopMovement();
		}

		else
		{
			GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;
			MovePos(&Destination);
		}
	}
	else
		LookY = 0;
}
