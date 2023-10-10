// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGLearnGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/VaultComponent.h"
#include "InputActionValue.h"
#include "Components/PlayerStatsComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARPGLearnGameCharacter

ARPGLearnGameCharacter::ARPGLearnGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	// Custom properties
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	IsCrouching = false;
	MaxWalkSpeedCrouched = 350.f;
	CurrentWalkSpeed = 500.f;

	// Sprint
	CanSprint = true;
	IsSprinting = false;

	// Motion Warping
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	// Vaulting
	VaultComponent = CreateDefaultSubobject<UVaultComponent>(TEXT("VaultComponent"));

	// Player Stats
	PlayerStatsComponent = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("PlayerStatsComponent"));
}

void ARPGLearnGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CrouchTimeline.TickTimeline(DeltaTime);
}

void ARPGLearnGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Crouch Timeline
	FOnTimelineFloat ProgressCrouchTimelineUpdate;
	ProgressCrouchTimelineUpdate.BindUFunction(this, FName("OnCrouchTimelineUpdate"));

	FOnTimelineEvent ProgressCrouchTimelineFinished;
	ProgressCrouchTimelineFinished.BindUFunction(this, FName("OnCrouchTimelineFinished"));

	CrouchTimeline.AddInterpFloat(CrouchCameraDistance, ProgressCrouchTimelineUpdate);
	CrouchTimeline.SetTimelineFinishedFunc(ProgressCrouchTimelineFinished);
	
	if(!IsSprinting)
	{
		GetWorldTimerManager().SetTimer(RestoreTime, this, &ARPGLearnGameCharacter::RestoreStamina, 1.f, true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(RestoreTime);
	}
}

float ARPGLearnGameCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	bool bPlayerIsDead = false;
	PlayerStatsComponent->DecreaseHealth(DamageAmount, bPlayerIsDead);
	if(bPlayerIsDead)
	{
		Die();
	}

	return DamageAmount;
}

void ARPGLearnGameCharacter::DrainStamina()
{
	if(CanSprint)
	{
		bool bPlayerIsDead = false;
		PlayerStatsComponent->DecreaseStamina(1.f, bPlayerIsDead);
		CanSprint = !bPlayerIsDead;
		IsSprinting = !bPlayerIsDead;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void ARPGLearnGameCharacter::RestoreStamina() const 
{
	PlayerStatsComponent->IncreaseStamina(1.f);
}

void ARPGLearnGameCharacter::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Player is dead"));
	DisableInput(Cast<APlayerController>(GetController()));
	GetMesh()->SetSimulatePhysics(true);
}

//////////////////////////////////////////////////////////////////////////
// Animation
void ARPGLearnGameCharacter::OnCrouchTimelineUpdate(float Value)
{
	// Set camera distance with lerp from 400 to 500
	CameraBoom->TargetArmLength = FMath::Lerp(400.f, 500.f, Value);
}

void ARPGLearnGameCharacter::OnCrouchTimelineFinished()
{
}

void ARPGLearnGameCharacter::Vault()
{
	VaultComponent->Vault();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPGLearnGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ARPGLearnGameCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGLearnGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGLearnGameCharacter::Look);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ARPGLearnGameCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ARPGLearnGameCharacter::StopCrouch);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARPGLearnGameCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARPGLearnGameCharacter::StopSprint);

		// Vaulting
		EnhancedInputComponent->BindAction(VaultAction, ETriggerEvent::Started, this, &ARPGLearnGameCharacter::Vault);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARPGLearnGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPGLearnGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARPGLearnGameCharacter::StartCrouch(const FInputActionValue& Value)
{
	IsCrouching = true;
	ARPGLearnGameCharacter::Crouch();
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeedCrouched;
	CrouchTimeline.PlayFromStart();	
}

void ARPGLearnGameCharacter::StopCrouch(const FInputActionValue& Value)
{
	IsCrouching = false;
	ARPGLearnGameCharacter::UnCrouch();
	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	CrouchTimeline.ReverseFromEnd();
}

void ARPGLearnGameCharacter::StartSprint(const FInputActionValue& Value)
{
	if(CanSprint && !bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeed = 750.f;
		
		GetWorld()->GetTimerManager().SetTimer(
			SprintTime,
			this,
			&ARPGLearnGameCharacter::DrainStamina,
			0.1f,
			true
		);
	}
}

void ARPGLearnGameCharacter::StopSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetWorldTimerManager().ClearTimer(SprintTime);
	IsSprinting = false;
}
