// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// �����ӿڣ�����Uǰ׺��
UINTERFACE(MinimalAPI, Blueprintable)
class UCombatInterface : public UInterface
{
    GENERATED_BODY()
};

// ʵ�ʽӿ��ࣨ��Iǰ׺��
class WUKONGGAS_API ICombatInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    bool IsDead() const; 

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    AActor* GetAvatar(); 

};