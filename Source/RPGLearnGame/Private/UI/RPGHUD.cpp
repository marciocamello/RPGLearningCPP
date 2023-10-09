// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPGHUD.h"

#include "Components/PlayerStatsComponent.h"
#include "Components/ProgressBar.h"

class UPlayerStatsComponent;

void URPGHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	UActorComponent* PlayerStatsComponent = GetOwningPlayerPawn()->GetComponentByClass(PlayerStatsComponentClass);
	if(PlayerStatsComponent)
	{
		UPlayerStatsComponent* PlayerStats = Cast<UPlayerStatsComponent>(PlayerStatsComponent);
		if(PlayerStats)
		{
			PlayerStats->OnUpdateHealthBar.AddUniqueDynamic(this,&URPGHUD::OnUpdateHealthBar);
			PlayerStats->OnUpdateStaminaBar.AddUniqueDynamic(this,&URPGHUD::OnUpdateStaminaBar);
		}
	}
}

void URPGHUD::OnUpdateHealthBar(float HealthPercentage)
{
	HealthBar->SetPercent(HealthPercentage);
}

void URPGHUD::OnUpdateStaminaBar(float StaminaPercentage)
{
	StaminaBar->SetPercent(StaminaPercentage);
}
