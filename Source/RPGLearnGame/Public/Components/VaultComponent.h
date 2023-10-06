// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VaultComponent.generated.h"

class ARPGLearnGameCharacter;

/** Please add a class description */
UCLASS(Blueprintable, BlueprintType)
class UVaultComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UVaultComponent();
	
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
};

