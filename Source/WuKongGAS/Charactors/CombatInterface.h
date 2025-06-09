// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// 声明接口（不带U前缀）
UINTERFACE(MinimalAPI, Blueprintable)
class UCombatInterface : public UInterface
{
    GENERATED_BODY()
};

// 实际接口类（带I前缀）
class WUKONGGAS_API ICombatInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    bool IsDead() const; 

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    AActor* GetAvatar(); 

};