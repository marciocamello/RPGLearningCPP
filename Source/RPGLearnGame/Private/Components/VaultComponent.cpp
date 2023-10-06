// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VaultComponent.h"

#include "KismetTraceUtils.h"
#include "MotionWarpingComponent.h"
#include "PlayMontageCallbackProxy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UVaultComponent::UVaultComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CanWarp = false;
	VaultMontage = nullptr;
}

void UVaultComponent::Vault()
{
	for(int CalculateDistanceIndex = 0; CalculateDistanceIndex <= 2; CalculateDistanceIndex++)
	{
		FVector CalculateDistanceStart = GetOwner()->GetActorLocation() + FVector(0.f, 0.f, CalculateDistanceIndex * 30);
		const FVector CalculateDistanceEnd = CalculateDistanceStart + (GetOwner()->GetActorRotation().Vector() * 180.f);

		FHitResult CalculateDistanceHitResult;

		UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			CalculateDistanceStart,
			CalculateDistanceEnd,
			5.f,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			{},
			EDrawDebugTrace::ForDuration,
			CalculateDistanceHitResult,
			true,
			FColor::Red,
			FColor::Green,
			5.f
		);

		if (CalculateDistanceHitResult.bBlockingHit)
		{
			for (int CalculateHeightIndex = 0; CalculateHeightIndex <= 5; CalculateHeightIndex++)
			{
				FVector CalculationHeightStartLocation = CalculateDistanceHitResult.Location + FVector(0.f, 0.f, 100.f);
				FVector CalculateHeightEndLocation = GetOwner()->GetActorRotation().Vector() * FVector(CalculateHeightIndex * 50, CalculateHeightIndex * 50, CalculateHeightIndex * 50);

				FVector CalculateHeightStart = CalculationHeightStartLocation + CalculateHeightEndLocation;
				FVector CalculateHeightEnd = CalculateHeightStart - FVector(0.f, 0.f, 100.f);

				FHitResult CalculateHeightHitResult;

				UKismetSystemLibrary::SphereTraceSingle(
					GetWorld(),
					CalculateHeightStart,
					CalculateHeightEnd,
					10.f,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					{},
					EDrawDebugTrace::ForDuration,
					CalculateHeightHitResult,
					true,
					FColor::Red,
					FColor::Green,
					5.f
				);

				if (CalculateHeightHitResult.bBlockingHit)
				{
					
					if(CalculateHeightIndex == 0)
					{
						VaultStartPos = CalculateHeightHitResult.Location;
						
						UKismetSystemLibrary::DrawDebugSphere(
							GetWorld(),
							VaultStartPos,
							10.f,
							12,
							FColor::Purple,
							5.f,
							2.f
						);
					}
					
					VaultMiddlePos = CalculateHeightHitResult.Location;
					
					UKismetSystemLibrary::DrawDebugSphere(
						GetWorld(),
						VaultMiddlePos,
						10.f,
						12,
						FColor::Yellow,
						5.f,
						2.f
					);

					CanWarp = true;
				}
				else
				{
					FVector LandPosLocationStart = CalculateHeightHitResult.TraceStart + (GetOwner()->GetActorRotation().Vector() * FVector(80.f, 80.f, 80.f));
					FVector LandPosLocationEnd = LandPosLocationStart - FVector(0.f, 0.f, 1000.f);
					
					FHitResult LandPosHitResult;
					
					UKismetSystemLibrary::LineTraceSingle(
						GetWorld(),
						LandPosLocationStart,
						LandPosLocationEnd,
						UEngineTypes::ConvertToTraceType(ECC_Visibility),
						false,
						{},
						EDrawDebugTrace::ForDuration,
						LandPosHitResult,
						true,
						FColor::Blue,
						FColor::Green,
						5.f
					);

					if(LandPosHitResult.bBlockingHit)
					{
						VaultLandPos = LandPosHitResult.Location;
						VaultMotionWarp();
						break;
					}
				}
			}
		}
	}
}

void UVaultComponent::VaultMotionWarp()
{
	USkeletalMeshComponent* Mesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	UCharacterMovementComponent* Movement = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
	UMotionWarpingComponent* MotionWarping = GetOwner()->GetComponentByClass<UMotionWarpingComponent>();

	const float MeshHeightMin = Mesh->GetComponentLocation().Z - 50.f;
	const float MeshHeightMax = Mesh->GetComponentLocation().Z + 50.f;
	const float MeshHeightValue = VaultLandPos.Z;

	const bool MeshHeightResult = UKismetMathLibrary::InRange_FloatFloat(MeshHeightValue, MeshHeightMin, MeshHeightMax);

	if(CanWarp && MeshHeightResult)
	{
		Movement->SetMovementMode(EMovementMode::MOVE_Flying);
		GetOwner()->SetActorEnableCollision(false);

		FMotionWarpingTarget VaultStartTarget;
		VaultStartTarget.Location = VaultStartPos;
		VaultStartTarget.Rotation = GetOwner()->GetActorRotation();
		VaultStartTarget.Name = "VaultStart";
		MotionWarping->AddOrUpdateWarpTarget(VaultStartTarget);
		
		FMotionWarpingTarget VaultMiddleTarget;
		VaultMiddleTarget.Location = VaultMiddlePos;
		VaultMiddleTarget.Rotation = GetOwner()->GetActorRotation();
		VaultMiddleTarget.Name = "VaultMiddle";
		MotionWarping->AddOrUpdateWarpTarget(VaultMiddleTarget);
		
		FMotionWarpingTarget VaultLandTarget;
		VaultLandTarget.Location = VaultLandPos;
		VaultLandTarget.Rotation = GetOwner()->GetActorRotation();
		VaultLandTarget.Name = "VaultLand";
		MotionWarping->AddOrUpdateWarpTarget(VaultLandTarget);
		
		UPlayMontageCallbackProxy* PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
			Mesh,
			VaultMontage
		);

		PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &UVaultComponent::OnVaultMontageCompleted);
	}
}

void UVaultComponent::OnVaultMontageCompleted(FName NotifyName)
{
	UCharacterMovementComponent* Movement = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
	Movement->SetMovementMode(EMovementMode::MOVE_Walking);
	GetOwner()->SetActorEnableCollision(true);
	CanWarp = false;
	VaultLandPos = FVector(0.f, 0.f, 20000.f);
}
