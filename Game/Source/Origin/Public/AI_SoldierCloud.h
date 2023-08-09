// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soldier.h"
#include "GameFramework/Character.h"
/**
 * 
 */
class ORIGIN_API AI_SoldierCloud
{
public:
	static AI_SoldierCloud * GetInstance();

	void Alarm(ASoldier * from, float distance);
	void Alarm(FVector location, float distance);	

	TArray<ASoldier*> Soldiers;

	void Clear();
	
	ACharacter* Player;

private:
	AI_SoldierCloud();
	~AI_SoldierCloud();
	static AI_SoldierCloud * Instance;

};
