// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/HeroAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPGLearnGame/RPGLearnGameCharacter.h"

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	DeltaTime = DeltaSeconds;
}

void UHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	GetCharacter();
	CalculateMovementParams();
	CalculateLeanDelta();
}

void UHeroAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UHeroAnimInstance::CalculateMovementParams()
{
	if(HeroCharacter)
	{
		const UCharacterMovementComponent* CharacterMovement = HeroCharacter->GetCharacterMovement();

		if(CharacterMovement)
		{
			IsFalling = CharacterMovement->IsFalling();
		}
		
		Speed = HeroCharacter->GetVelocity().Length();
		Velocity = HeroCharacter->GetVelocity();
		IsCrouching = HeroCharacter->IsCrouching;
	}
}

void UHeroAnimInstance::GetCharacter()
{
	HeroCharacter = Cast<ARPGLearnGameCharacter>(TryGetPawnOwner());
}

void UHeroAnimInstance::CalculateLeanDelta()
{
	if(HeroCharacter)
	{
		const FRotator HeroRotation = HeroCharacter->GetActorRotation();
		FRotator DeltaRotator = HeroRotation - LastFrameRotation;
		DeltaRotator.Normalize();
		const float NormalizedRotatorYaw = DeltaRotator.Yaw;
		const float DeltaCalculated = (NormalizedRotatorYaw / DeltaTime) / 10;
		YawnDelta = FMath::FInterpTo(YawnDelta, DeltaCalculated, DeltaTime, 10);
		LastFrameRotation = HeroRotation;
	}
}
