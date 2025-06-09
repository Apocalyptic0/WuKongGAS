// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASCharacterBase.h"
#include "CombatInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "../AttributeSets/CharacterAttributeSetBase.h"
#include "../Interfaces/TargetInterface.h"
#include "GASEnemyBase.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewAttribute);

UCLASS()
class WUKONGGAS_API AGASEnemyBase : public AGASCharacterBase, public ICombatInterface, public ITargetInterface
{
	GENERATED_BODY()

public:

	AGASEnemyBase();
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual void Select_Implementation(FTransform& TargetMarkRelativeTrans) override;
	virtual void CancelSelect_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UWidgetComponent> TargetMark;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	bool bDead = false;
};
