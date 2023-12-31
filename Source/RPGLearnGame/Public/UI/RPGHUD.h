// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGHUD.generated.h"

class UActorComponent;
class UImage;
class UProgressBar;

/**
 * 
 */
UCLASS()
class RPGLEARNGAME_API URPGHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* HeroImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	TSubclassOf<UActorComponent> PlayerStatsComponentClass;

	UFUNCTION(BlueprintCallable)
	void OnUpdateHealthBar(float HealthPercentage);

	UFUNCTION(BlueprintCallable)
	void OnUpdateStaminaBar(float StaminaPercentage);
};
