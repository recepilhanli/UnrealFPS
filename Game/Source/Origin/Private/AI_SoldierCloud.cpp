// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_SoldierCloud.h"

AI_SoldierCloud::AI_SoldierCloud()
{
}

AI_SoldierCloud::~AI_SoldierCloud()
{
}

AI_SoldierCloud *AI_SoldierCloud::Instance = nullptr;

AI_SoldierCloud *AI_SoldierCloud::GetInstance()
{

    if (Instance == nullptr)
        Instance = new AI_SoldierCloud();

    return Instance;
}

void AI_SoldierCloud::Alarm(FVector Location, float distance)
{
    for (ASoldier *soldier : Soldiers)
    {

        FVector soldierLoc = soldier->GetActorLocation();
        float currentdist = FVector::Distance(Location, soldierLoc);

        if (distance > currentdist && (soldier->MovingPos == FVector::Zero()) && (soldier->Seen == nullptr))
        {

            FVector Direction = Location - soldier->GetActorLocation();
            FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
            NewRotation.Pitch = 0;
            NewRotation.Roll = 0;
            soldier->SetActorRotation(NewRotation);
            soldier->InDanger = true;
            if (distance > 500)
                soldier->Roaming(Location, 300);
        }
    }
}

void AI_SoldierCloud::Alarm(ASoldier *from, float distance)
{

    FVector Location = from->GetActorLocation();

    for (ASoldier *soldier : Soldiers)
    {

        if (soldier == from)
            continue;

        FVector soldierLoc = soldier->GetActorLocation();
        float currentdist = FVector::Distance(Location, soldierLoc);

        if (distance < currentdist)
        {
            if (distance * 2.5f > currentdist)
                soldier->InDanger = true;

            if ((soldier->MovingPos == FVector::Zero()) && (soldier->Seen == nullptr) && distance * 2.0f > currentdist)
            {

                soldier->Roaming();
            }

            continue;
        }

        if (soldier->Seen == nullptr)
        {
            soldier->LastSeen = from->LastSeen;
            soldier->Seen = from->Seen;
        }
    }
}

void AI_SoldierCloud::Clear()
{
    Soldiers.Reset();

    delete Instance;
    Instance = nullptr;
}