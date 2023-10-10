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

public:

	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Health functions
	UFUNCTION(BlueprintCallable, Category = "Health")
	void DecreaseHealth(const float Damage, bool& IsPlayerDead);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseMaxHealth(const float NewMaxHealth);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseHealth(const float NewHealth);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth() const;
	
	// Stamina functions
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void DecreaseStamina(const float StaminaCost, bool& IsPlayerDead);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void IncreaseMaxStamina(const float NewMaxStamina);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void IncreaseStamina(const float NewStamina);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateStamina();

	// HUD widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<URPGHUD> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	URPGHUD* HUDWidget;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateHealthBarSignature OnUpdateHealthBar;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateStaminaBarSignature OnUpdateStaminaBar;
	
	inline float GetCurrentHealth() const { return CurrentHealth; }
	inline float GetMaxHealth() const { return MaxHealth; }
	inline float GetCurrentStamina() const { return CurrentStamina; }
	inline float GetMaxStamina() const { return MaxStamina; }
};
