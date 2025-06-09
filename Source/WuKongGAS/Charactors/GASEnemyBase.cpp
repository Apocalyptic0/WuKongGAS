// Fill out your copyright notice in the Description page of Project Settings.


#include "GASEnemyBase.h"

AGASEnemyBase::AGASEnemyBase()
{
	TargetMark = CreateDefaultSubobject<UWidgetComponent>("TargetMark");
	TargetMark->SetupAttachment(GetRootComponent());

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

bool AGASEnemyBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AGASEnemyBase::GetAvatar_Implementation()
{
	return this;
}

void AGASEnemyBase::Select_Implementation(FTransform& TargetMarkRelativeTrans)
{
	if (TargetMark)
	{
		FTransform TargetMarkTrans = TargetMark->GetComponentTransform();
		FTransform CharacterTrans = GetActorTransform();
		TargetMarkRelativeTrans = TargetMarkTrans.GetRelativeTransform(CharacterTrans);
		TargetMark->SetVisibility(true);
	}
}

void AGASEnemyBase::CancelSelect_Implementation()
{
	if (TargetMark)
	{
		TargetMark->SetVisibility(false);
	}
}

void AGASEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	const UCharacterAttributeSetBase* AttributeSet = Cast<UCharacterAttributeSetBase>(AbilitySystemComponent->GetAttributeSet(UCharacterAttributeSetBase::StaticClass()));
	if (AttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AttributeSet]()
		{
			OnHealthChanged.Broadcast(AttributeSet->GetHealth());
			OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());
		}, 0.1f, false);
	}
}
