// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCharacterBase.h"


// Sets default values
AGASCharacterBase::AGASCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
}

// Called when the game starts or when spawned
void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();

    if (AbilitySystemComponent == nullptr) { return; }

    for (auto i = 0; i < Abilities.Num(); i++)
    {
        if (Abilities[i] == nullptr) { continue; }
        AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Abilities[i].GetDefaultObject(), 1, 0));
    }

    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    for (TSubclassOf<UAttributeSet>& Set : AttributeSets)
    {
        AbilitySystemComponent->InitStats(Set, AttrDataTable);
    }
}

// Called every frame
void AGASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

