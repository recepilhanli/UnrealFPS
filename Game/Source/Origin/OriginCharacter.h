// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI_Interface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "LegacyCameraShake.h"
#include "OriginCharacter.generated.h"

#define CAMSHAKE_IDLE (0)
#define CAMSHAKE_WALK (1)
#define CAMSHAKE_RUN (2)
#define CAMSHAKE_FIRE (3)
#define CAMSHAKE_DAMAGE (4)

UCLASS(config = Game)
class AOriginCharacter : public ACharacter, public IAI_Interface
{
	GENERATED_BODY()

	// /** Camera boom positioning the camera behind the character */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *FPSCamera;

	UPROPERTY(EditAnywhere)
	class USpotLightComponent *Flashlight;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext *DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *LookAction;

	/** Camera Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *CameraAction;

	/** FlashLight Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *FlashlightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *CrouchAction;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	class UCapsuleComponent *TriggerCapsule;

	UPROPERTY(EditAnywhere, Category = "Player")
	UStaticMeshComponent *WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UParticleSystemComponent *MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UPointLightComponent *MuzzleLight;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UAudioComponent *PlayerAudio1;

	UPROPERTY(EditAnywhere, Category = "Player")
	TArray<TSubclassOf<class ULegacyCameraShake>> Shakes;

public:
	AOriginCharacter();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue &Value);

	/** Called for looking input */
	void Look(const FInputActionValue &Value);

	void ToggleFlashlight();

	void ToggleCamera();

	void Fire();

	void ToggleFire(bool toggle);
	void FireEffects();

	void Zoom();
	void MovementShake();

	UCameraComponent *GetCurrentCamera();

	UFUNCTION(BlueprintCallable)
	void StartSwimming();
	UFUNCTION(BlueprintCallable)
	void StopSwimming();

	void StartJumping();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	// To add mapping context
	virtual void Tick(float DeltaSeconds);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	// virtual void MovePos(FVector ToPosition) override;
	virtual void GiveHealth(float h, FVector from = FVector::ZeroVector) override;
	virtual void Kill() override;
	virtual void MovePos(FVector *ToPosition) override;
	virtual void MakeSound(int soundtype) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABulletActor> BulletRef;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Hud;
	UUserWidget *HudInstance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent *GetFollowCamera() const { return FPSCamera; }

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	float FireRate = 0;
	bool Swimming = false;
	bool Aiming = false;
	int WeaponID = 0;
};
