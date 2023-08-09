// Copyright Epic Games, Inc. All Rights Reserved.

#include "OriginCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "BulletActor.h"
#include "Weapons.h"
#include "AI_SoldierCloud.h"
#include "SoundManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "AnimMontageManager.h"

//////////////////////////////////////////////////////////////////////////
// AOriginCharacter

AOriginCharacter::AOriginCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	PlayerAudio1 = CreateDefaultSubobject<UAudioComponent>(TEXT("Player Audio 1"));
	PlayerAudio1->SetupAttachment(RootComponent);
	PlayerAudio1->SetAutoActivate(true);

	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(GetCapsuleComponent());
	FPSCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FPSCamera);

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);

	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(FPSCamera);
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("hand_l"));
	WeaponMesh->CastShadow = false;
	SET_WEAPON(WeaponMesh,WeaponID);


	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash"));
	MuzzleFlash->SetupAttachment(WeaponMesh, FName("Muzzle"));

	MuzzleLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Muzzle Light"));
	MuzzleLight->SetupAttachment(MuzzleFlash);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AOriginCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AOriginCharacter::OnOverlapEnd);
}

void AOriginCharacter::MakeSound(int soundtype)
{

	if (soundtype == SOUNDTYPE_FOOTSTEP)
	{
		if (Swimming)
			return;

		PLAY_SOUND_2D(SOUND_PLAYER_STEP_SAND);
	}
}

void AOriginCharacter::OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag(TEXT("Water")))
		{
			StartSwimming();
		}
	}
}

void AOriginCharacter::OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{

		if (OtherActor->ActorHasTag(TEXT("Water")))
		{
			StopSwimming();
		}
	}
}

void AOriginCharacter::StartSwimming()
{
	GetCharacterMovement()->MovementMode = MOVE_Swimming;
	APhysicsVolume *vol = GetCharacterMovement()->GetPhysicsVolume();
	vol->bWaterVolume = true;
	Swimming = true;
}

void AOriginCharacter::StopSwimming()
{
	GetCharacterMovement()->MovementMode = MOVE_Walking;
	APhysicsVolume *vol = GetCharacterMovement()->GetPhysicsVolume();
	vol->bWaterVolume = false;
	Swimming = false;
}

void AOriginCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	MuzzleFlash->SetVisibility(false);
	MuzzleLight->SetVisibility(false);

	AI_SoldierCloud *cloud = AI_SoldierCloud::GetInstance();
	cloud->Player = this;

	Weapons *wepinstance = Weapons::GetInstance();
	wepinstance->SetBulletRef(BulletRef);

	GetCurrentCamera()->FieldOfView = 90;
	bUseControllerRotationYaw = true;

	HudInstance = CreateWidget<UUserWidget>(GetWorld(), Hud);
	HudInstance->AddToViewport();

	if (Flashlight)
	{
		Flashlight->SetVisibility(false);
	}

	// Add Input Mapping Context
	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AOriginCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// For Editor
	SoundManager *soundmanager = SoundManager::GetInstance();
	soundmanager->Clear();

	AI_SoldierCloud *cloud = AI_SoldierCloud::GetInstance();
	cloud->Clear();

	// AnimMontageManager *montagemanager = AnimMontageManager::GetInstance();
	// montagemanager->Clear();
}

void AOriginCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MovementShake();

	float Milliseconds = GetWorld()->TimeSeconds * 1000.0f;
	if (FireRate != 0 && Milliseconds > FireRate)
	{
		float rate = Weapons::GetInstance()->GetWeapon(WeaponID)->Rate;
		FireRate = Milliseconds + rate;
		Fire();
	}

	if (Aiming == false && GetCurrentCamera()->FieldOfView != 90)
		GetCurrentCamera()->FieldOfView = FMath::Lerp(GetCurrentCamera()->FieldOfView, 90, DeltaSeconds * 10);
	else if (GetCurrentCamera()->FieldOfView != 75)
		GetCurrentCamera()->FieldOfView = FMath::Lerp(GetCurrentCamera()->FieldOfView, 70, DeltaSeconds * 10);
}

void AOriginCharacter::MovePos(FVector *ToPosition) // Unused
{
}

void AOriginCharacter::GiveHealth(float h, FVector from)
{
	Health += h;

	if (h < 0)
	{
		PLAY_SOUND_2D(SOUND_PLAYER_TAKEDAMAGE);
	}

	if (Health <= 0)
	{
		Kill();
	}
}

void AOriginCharacter::Kill()
{
}

//////////////////////////////////////////////////////////////////////////
// Input

void AOriginCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOriginCharacter::StartJumping);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOriginCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOriginCharacter::Look);

		// Other Features
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AOriginCharacter::ToggleFlashlight);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AOriginCharacter::ToggleFire, true);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AOriginCharacter::ToggleFire, false);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &AOriginCharacter::Zoom);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ACharacter::Crouch, true);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACharacter::UnCrouch, true);
	}
}

void AOriginCharacter::ToggleFire(bool toggle)
{
	if (toggle && FireRate == 0)
		FireRate = 1;
	else if (!toggle)
		FireRate = 0;
}

void AOriginCharacter::MovementShake()
{
	float speed = GetVelocity().Length();

	if (speed < 250 || GetCharacterMovement()->IsFalling())
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(Shakes[CAMSHAKE_IDLE]);

	else if (speed < 350)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(Shakes[CAMSHAKE_WALK]);

	else
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(Shakes[CAMSHAKE_RUN], 0.075F);
}

void AOriginCharacter::StartJumping()
{
	if (Swimming)
		return;

	if (!GetCharacterMovement()->IsFalling())
	{
		PLAY_SOUND(PlayerAudio1, SOUND_PLAYER_JUMP);
	}

	Jump();
}

void AOriginCharacter::ToggleFlashlight()
{
	if (Flashlight)
		Flashlight->SetVisibility(!Flashlight->IsVisible());
}

void AOriginCharacter::Fire()
{

	AI_SoldierCloud::GetInstance()->Alarm(GetActorLocation(), 5000);

	PLAY_MONTAGE(GetMesh(), MONTAGE_FIRE);

	const FRotator Rotation = GetCurrentCamera()->GetComponentRotation();
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));

	ABulletActor *MyBullet = GetWorld()->SpawnActor<ABulletActor>(BulletRef, MuzzleLocation, Rotation);
	if (MyBullet)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(Shakes[CAMSHAKE_FIRE], 0.5f);
		FTimerHandle LifetimeTimerHandle;
		float Lifetime = 0.125;
		GetWorldTimerManager().SetTimer(LifetimeTimerHandle, this, &AOriginCharacter::FireEffects, Lifetime, false);
		MuzzleFlash->SetVisibility(true);
		MuzzleLight->SetVisibility(true);
		PLAY_SOUND_2D(SOUND_WEAPON_AR_FIRE);
		MyBullet->ProjectileMovement->Velocity = GetCurrentCamera()->GetForwardVector() * MyBullet->ProjectileMovement->InitialSpeed;

		UWeaponData * wep = Weapons::GetInstance()->GetWeapon(WeaponID);

		float x = (FMath::RandRange((float)-(wep->Recoil.X),(float)wep->Recoil.X))*1.25f;
		float y = -(FMath::RandRange(0.05f,(float)wep->Recoil.Y))*1.5f;

		AddControllerYawInput(x);
		AddControllerPitchInput(y);


	}

	// GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(WeaponShake, 1.0f);
}

void AOriginCharacter::FireEffects()
{

	MuzzleFlash->SetVisibility(false);
	MuzzleLight->SetVisibility(false);
}

void AOriginCharacter::Zoom()
{
	Aiming = !Aiming;
}

UCameraComponent *AOriginCharacter::GetCurrentCamera()
{
	// if (FollowCamera->IsActive())
	// 	return FollowCamera;

	return FPSCamera;
}

void AOriginCharacter::Move(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		if (Swimming)
		{
			const FVector Move = Controller->GetControlRotation().Vector();
			AddMovementInput(Move, MovementVector.Y);
		}
		else
		{
			AI_SoldierCloud::GetInstance()->Alarm(GetActorLocation(), 500);
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(ForwardDirection, MovementVector.Y);
		}
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AOriginCharacter::Look(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
