// Fill out your copyright notice in the Description page of Project Settings.
#include "TargetComponent.h"


// Sets default values for this component's properties
UTargetComponent::UTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TargetActor)
	{
		const bool ImplementsCombatInterfaceAndDead = TargetActor->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(TargetActor);
		if (ImplementsCombatInterfaceAndDead)
		{
			CancelTarget();
			return ;
		}

		ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (MyCharacter && PC)
		{
			const APlayerCameraManager* CameraManager = PC->PlayerCameraManager;
			FVector StartLocation = CameraManager->GetCameraLocation();
			FTransform TargetTrans = TargetMarkRelativeTrans * TargetActor->GetTransform(); 
			FVector TargetLocation = TargetTrans.GetLocation();

			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
			TargetRotation.Roll = 0.0f;
			TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch, -10.0f, 10.0f);

			FRotator NewRotation = UKismetMathLibrary::RInterpTo_Constant(MyCharacter->GetControlRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 200.0f);

			MyCharacter->GetController()->SetControlRotation(NewRotation);
		}
	}
}

void UTargetComponent::StoreRotationMode()
{
	ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
	if (MyCharacter == nullptr)
	{
		return;
	}

	bUseControllerRotationYaw = MyCharacter->bUseControllerRotationYaw;
	bOrientRotationToMovement = MyCharacter->GetCharacterMovement()->bOrientRotationToMovement;
	bUseControllerDesiredRotation = MyCharacter->GetCharacterMovement()->bUseControllerDesiredRotation;

	if (AController* Controller = MyCharacter->GetController())
	{
		Controller->SetIgnoreLookInput(true);
	}
}

void UTargetComponent::RestoreRotationMode()
{
	ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
	if (MyCharacter == nullptr)
	{
		return;
	}

	MyCharacter->bUseControllerRotationYaw = bUseControllerRotationYaw;
	MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
	MyCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = bUseControllerDesiredRotation;

	if (AController* Controller = MyCharacter->GetController())
	{
		Controller->SetIgnoreLookInput(false);
	}

}

AActor* UTargetComponent::GetTarget()
{
	return TargetActor;
}

void UTargetComponent::BindTarget(AActor* Target)
{
	if(TargetActor != nullptr)
	{
		bool ImplementsTargetInterface = TargetActor->Implements<UTargetInterface>();
		if (ImplementsTargetInterface)
		{
			ITargetInterface::Execute_CancelSelect(TargetActor);
		}
	}
	else
	{
		StoreRotationMode();
	}

	bool ImplementsTargetInterface = Target->Implements<UTargetInterface>();
	if (ImplementsTargetInterface)
	{
		TargetActor = Target;
		ITargetInterface::Execute_Select(Target, TargetMarkRelativeTrans);
	}
}

void UTargetComponent::CancelTarget()
{
	RestoreRotationMode();
	bool ImplementsTargetInterface = TargetActor->Implements<UTargetInterface>();
	if (ImplementsTargetInterface)
	{
		ITargetInterface::Execute_CancelSelect(TargetActor);
	}
	TargetActor = nullptr;
}

void UTargetComponent::LockCharacterLootAt()
{
	if (ACharacter* MyCharacter = Cast<ACharacter>(GetOwner()))
	{
		MyCharacter->bUseControllerRotationYaw = true;
		MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		MyCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void UTargetComponent::UnlockCharacterLookAt()
{
	if (ACharacter* MyCharacter = Cast<ACharacter>(GetOwner()))
	{
		MyCharacter->bUseControllerRotationYaw = false;
		MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		MyCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

