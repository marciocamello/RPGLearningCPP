// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "Components/ActorComponent.h"
#include "VaultComponent.generated.h"

class ARPGLearnGameCharacter;
class UMotionWarpingComponent;

UCLASS(Blueprintable, BlueprintType)
class UVaultComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* Movement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarping;
	
public:
	UVaultComponent();
	void VaultTraceStartPos(int CalculateHeightIndex, const FHitResult& CalculateHeightHitResult);
	void VaultTraceMiddlePos(const FHitResult& CalculateHeightHitResult);
	void VaultTraceLandPos(const FHitResult& CalculateHeightHitResult, FHitResult& LandPosHitResult) const;

	UFUNCTION(BlueprintCallable, Category="Default")
	void Vault();

	UFUNCTION(BlueprintCallable, Category="Default")
	void VaultMotionWarp();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector VaultStartPos;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector VaultMiddlePos;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector VaultLandPos;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool CanWarp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	UAnimMontage* VaultMontage;

private:
	UFUNCTION()
	void OnVaultMontageCompleted(FName NotifyName);
	
protected:
	void VaultCalculateHeight(FHitResult CalculateDistanceHitResult);
	void ApplyVaultMotionWarp(const FVector& MotionLocation, const FName MotionName) const;
};

