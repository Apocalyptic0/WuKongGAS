// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Charactors/CombatInterface.h"
#include "../Interfaces/TargetInterface.h"

#include "TargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WUKONGGAS_API UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void StoreRotationMode();
	void RestoreRotationMode();

	bool bUseControllerRotationYaw;
	bool bOrientRotationToMovement;
	bool bUseControllerDesiredRotation;
	FTransform TargetMarkRelativeTrans;
	AActor* TargetActor;

public:
	AActor* GetTarget();
	void BindTarget(AActor* Target);
	void CancelTarget();
	void LockCharacterLootAt();
	void UnlockCharacterLookAt();
	
};
