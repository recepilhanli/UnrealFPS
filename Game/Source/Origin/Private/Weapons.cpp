// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons.h"

Weapons *Weapons::Instance = nullptr;

Weapons *Weapons::GetInstance()
{
    if (Instance == nullptr)
    {
        Instance = new Weapons();
    }
    return Instance;
}

UWeaponData *Weapons::GetWeapon(int id)
{
    if (WeaponList[id] == nullptr)
        throw "Invalid weapon ID!";
    return WeaponList[id];
}

void Weapons::SetBulletRef(TSubclassOf<ABulletActor> bulletref)
{
    BulletClass = bulletref;
}

void Weapons::GetAllWeapons()
{
    UWeaponData *wep = Cast<UWeaponData>(StaticLoadObject(UWeaponData::StaticClass(), nullptr, TEXT("/Game/Weapons/Datas/AR4")));
    WeaponList.Add(wep);

    wep = Cast<UWeaponData>(StaticLoadObject(UWeaponData::StaticClass(), nullptr, TEXT("/Game/Weapons/Datas/UZI")));
    WeaponList.Add(wep);

    wep = Cast<UWeaponData>(StaticLoadObject(UWeaponData::StaticClass(), nullptr, TEXT("/Game/Weapons/Datas/UZI")));
    WeaponList.Add(wep);
}

Weapons::Weapons()
{
    GetAllWeapons();
}

Weapons::~Weapons()
{
}
