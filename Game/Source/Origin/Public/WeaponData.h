// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class ORIGIN_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	

	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStaticMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Rate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MaxClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector2D Recoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FTransform CameraTransform;

};
