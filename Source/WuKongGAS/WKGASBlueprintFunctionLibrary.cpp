// Fill out your copyright notice in the Description page of Project Settings.


#include "WKGASBlueprintFunctionLibrary.h"

void UWKGASBlueprintFunctionLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore); 

	TArray<FOverlapResult> Overlaps; 
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) //获取当前所处的场景，如果获取失败，将打印并返回Null
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps) 
		{
			//判断当前Actor是否包含战斗接口   Overlap.GetActor() 从碰撞检测结果中获取到碰撞的Actor
			const bool ImplementsCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			//判断当前Actor是否存活
			if (ImplementsCombatInterface && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

bool UWKGASBlueprintFunctionLibrary::GetNearestTargetWithLine(const UObject* WorldContextObject, AActor*& OutTarget, const APlayerCameraManager* CameraManager, const float CameraOffset, const float MaxDistance, const TArray<AActor*>& ActorsToIgnore)
{
	FVector Start = CameraManager->GetCameraLocation() + CameraManager->GetCameraRotation().Vector() * CameraOffset;
	FVector End = Start + CameraManager->GetCameraRotation().Vector() * MaxDistance;

	ECollisionChannel CollisionChannel = ECC_Pawn;
	TArray<FHitResult> HitResults;
	bool bFindTarget = false;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	bool const bHit = World ? World->LineTraceMultiByChannel(HitResults, Start, End, CollisionChannel, CollisionParams) : false;

	if (bHit)
	{
		float MinDistance = FLT_MAX;

		for (const FHitResult& Hit : HitResults)
		{
			AActor* TargetActor = Hit.GetActor();
			
			const bool ImplementsTargetInterface = TargetActor->Implements<UTargetInterface>();
			const bool NotImplementsCombatInterfaceOrNotDead = (TargetActor->Implements<UCombatInterface>()) ? (!ICombatInterface::Execute_IsDead(TargetActor)) : true;
			if (ImplementsTargetInterface && NotImplementsCombatInterfaceOrNotDead)
			{
				float CurrentDistance = (Hit.Location - Start).Length();
				if (CurrentDistance < MinDistance)
				{
					MinDistance = CurrentDistance;
					OutTarget = TargetActor;
					bFindTarget = true;
				}
			}
		}
	}

	return bFindTarget;
}

bool UWKGASBlueprintFunctionLibrary::GetTargetWithSphere(const UObject* WorldContextObject, AActor*& OutTarget, const APlayerCameraManager* CameraManager, const float CameraOffset, const float MaxDistance, const float MaxAngle, const TArray<AActor*>& ActorsToIgnore, int32 Direction, const float DistanceWeight, const float AngleWeight)
{
	TArray<AActor*> OverlappingActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 检测的碰撞通道
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	FVector CameraLocation = CameraManager->GetCameraLocation() + CameraManager->GetCameraRotation().Vector() * CameraOffset;
	FVector CameraForward = CameraManager->GetCameraRotation().Vector();
	FVector CameraRight = FRotationMatrix(CameraManager->GetCameraRotation()).GetScaledAxis(EAxis::Y);

	bool bFindTarget = false;
	float MaxScore = 0.0f;

	UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, CameraLocation, MaxDistance, ObjectTypes, nullptr, ActorsToIgnore, OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		const bool ImplementsTargetInterface = OverlappingActor->Implements<UTargetInterface>();
		const bool NotImplementsCombatInterfaceOrNotDead = (OverlappingActor->Implements<UCombatInterface>()) ? (!ICombatInterface::Execute_IsDead(OverlappingActor)) : true;
		if (ImplementsTargetInterface && NotImplementsCombatInterfaceOrNotDead)
		{
			FVector ActorDirection = (OverlappingActor->GetActorLocation() - CameraLocation).GetSafeNormal();

			// Dirction
			float DotForward = FVector::DotProduct(CameraForward, ActorDirection);
			bool bIsInFront = DotForward > 0;

			float DotRight = FVector::DotProduct(CameraRight, ActorDirection);
			bool bIsInRight = DotRight > 0;

			if ((Direction & ETargetDirection::FRONT) == 0 && bIsInFront
				|| (Direction & ETargetDirection::BACK) == 0 && !bIsInFront
				|| (Direction & ETargetDirection::LEFT) == 0 && !bIsInRight
				|| (Direction & ETargetDirection::RIGHT) == 0 && bIsInRight)
			{
				continue;
			}

			// Distance
			float Distance = FVector::Distance(CameraLocation, OverlappingActor->GetActorLocation());
			float NormDistance = FMath::Clamp(Distance / MaxDistance, 0.1f, 1.0f);

			// Angle
			float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CameraForward, ActorDirection)));
			float NormAngle = FMath::Clamp(Angle / MaxAngle, 0.0f, 1.0f);

			// ChooseTarget
			float Score = 1.0f / ((NormDistance * DistanceWeight) + (NormAngle * AngleWeight));
			if (Score > MaxScore)
			{
				MaxScore = Score;
				OutTarget = OverlappingActor;
				bFindTarget = true;
			}
		}
	}
	
	return bFindTarget;
}
