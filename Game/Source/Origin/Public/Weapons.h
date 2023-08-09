// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "BulletActor.h"


/**
 * 
 */

#define SET_WEAPON(mesh,weaponid)\
{\
UWeaponData * wep = Weapons::GetInstance()->GetWeapon(weaponid);\
mesh->SetStaticMesh(wep->WeaponMesh);\
mesh->SetRelativeTransform(wep->CameraTransform);\
}


class ORIGIN_API Weapons
{

public:
    static Weapons* GetInstance();
	void SetBulletRef(TSubclassOf<ABulletActor> bulletref);
	UWeaponData* GetWeapon(int id);
	TSubclassOf<ABulletActor> BulletClass;
	

private:
	Weapons();
	~Weapons();
	void GetAllWeapons();

	TArray<UWeaponData*> WeaponList;
    static Weapons* Instance;

};

