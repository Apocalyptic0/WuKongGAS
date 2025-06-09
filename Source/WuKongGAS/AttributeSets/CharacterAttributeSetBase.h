// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class WUKONGGAS_API UCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health = 100.0f;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Health);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth = 100.0f;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData WalkSpeed = 300.0f;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, WalkSpeed)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData RunSpeed = 500.0f;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, RunSpeed)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Speed = 300.0f;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Speed)
};
