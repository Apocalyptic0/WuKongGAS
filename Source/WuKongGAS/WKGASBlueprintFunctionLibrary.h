// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KismetTraceUtils.h"
#include "Engine/OverlapResult.h"
#include "Charactors/CombatInterface.h"
#include "Interfaces/TargetInterface.h"
#include "WKGASBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ETargetDirection : uint32
{
	None = 0 UMETA(Hidden),
	LEFT  = 0x01 UMETA(DisplayName = "Left"),
	RIGHT = 0x02 UMETA(DisplayName = "Right"),
	FRONT = 0x04 UMETA(DisplayName = "Front"),
	BACK  = 0x08 UMETA(DisplayName = "Back"),
};

ENUM_CLASS_FLAGS(ETargetDirection);

UCLASS()
class WUKONGGAS_API UWKGASBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "MyGASLibrary|Gameplay")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintCallable, Category = "MyGASLibrary|Gameplay")
	static bool GetNearestTargetWithLine(const UObject* WorldContextObject, AActor*& OutTarget,const APlayerCameraManager* CameraManager, const float CameraOffset, const float MaxDistance, const TArray<AActor*>& ActorsToIgnore);

	UFUNCTION(BlueprintCallable, Category = "MyGASLibrary|Gameplay")
	static bool GetTargetWithSphere(const UObject* WorldContextObject, AActor*& OutTarget,const APlayerCameraManager* CameraManager, const float CameraOffset, const float MaxDistance, const float MaxAngle, const TArray<AActor*>& ActorsToIgnore, int32 Direction, const float DistanceWeight, const float AngleWeight);
};
