// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/TimelineComponent.h"
#include "MotionWarpingComponent.h"
#include "RPGLearnGameCharacter.generated.h"

class UPlayerStatsComponent;
class USpringArmComponent;
class UCameraComponent;
class UMotionWarpingComponent;
class UInputMappingContext;
class UVaultComponent;
class UInputAction;
class UCurveFloat;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_EVENT(FMyDamageEvent, FAnyDamageReceivedEvent);

UCLASS(config=Game)
class ARPGLearnGameCharacter : public ACharacter
{
	GENERATED_BODY()

	// Components
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// Motion Warping
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Locomotion", meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpingComponent;

	// Vaulting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Locomotion", meta = (AllowPrivateAccess = "true"))
	UVaultComponent* VaultComponent;

	// Player Stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Stats", meta = (AllowPrivateAccess = "true"))
	UPlayerStatsComponent* PlayerStatsComponent;
	
	// Inputs
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	
	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Vault Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VaultAction;

public:
	ARPGLearnGameCharacter();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Locomotion")
	UCurveFloat* CrouchCameraDistance = nullptr;

private:
	FTimeline CrouchTimeline;

	UFUNCTION()
	void OnCrouchTimelineUpdate(float Value);

	UFUNCTION()
	void OnCrouchTimelineFinished();
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for crouching input */
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);

	/** Called for sprint input */
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);

	void Vault(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

private:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Get private, protected properties

	// Common
	UPROPERTY(BlueprintReadWrite, Category = "Character|Locomotion")
	float CurrentWalkSpeed;

	// Crouching
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Locomotion")
	bool IsCrouching;

	UPROPERTY(BlueprintReadWrite, Category = "Character|Locomotion")
	float MaxWalkSpeedCrouched;
	
	// Sprinting
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Locomotion")
	bool CanSprint;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Locomotion")
	bool IsSprinting;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Locomotion")
	FTimerHandle SprintTime;

	UFUNCTION()
	void DrainStamina();
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Locomotion")
	FTimerHandle RestoreTime;

	UFUNCTION()
	void RestoreStamina() const;

	// Die
	void Die();

	// Vaulting
	FORCEINLINE class UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }
};