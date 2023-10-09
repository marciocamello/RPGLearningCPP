// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHealthBarSignature, float, HealthPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateStaminaBarSignature, float, StaminaPercentage);

class URPGHUD;

UCLASS(Blueprintable, BlueprintType)
class RPGLEARNGAME_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatsComponent();

protected:
	virtual void BeginPlay() override;

	// Health Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;

	// Stamina Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.f;

	// Health functions
	UFUNCTION(BlueprintCallable, Category = "Health")
	void DecreaseHealth(float Damage, bool& IsPlayerDead);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHealth(float NewMaxHealth);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth();
	
	// Stamina functions
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void DecreaseStamina(float StaminaCost);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void IncreaseMaxStamina(float NewMaxStamina);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void IncreaseStamina(float NewStamina);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateStamina();

	// HUD widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<URPGHUD> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	URPGHUD* HUDWidget;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateHealthBarSignature OnUpdateHealthBar;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateStaminaBarSignature OnUpdateStaminaBar;
};
