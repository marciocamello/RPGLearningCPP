#include "Components/PlayerStatsComponent.h"

#include "Components/ProgressBar.h"
#include "UI/RPGHUD.h"

UPlayerStatsComponent::UPlayerStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateHealth();
	UpdateStamina();
}

void UPlayerStatsComponent::DecreaseHealth(float Damage, bool& IsPlayerDead)
{
	if(CurrentHealth <= 0.f)
	{
		IsPlayerDead = true;
	}
	else
	{
		CurrentHealth = CurrentHealth - Damage;
		UpdateHealth();
		IsPlayerDead = (CurrentHealth <= 0.f);
	}
}

void UPlayerStatsComponent::IncreaseMaxHealth(float NewMaxHealth)
{
	MaxHealth = MaxHealth + NewMaxHealth;
	UpdateHealth();
}

void UPlayerStatsComponent::IncreaseHealth(float NewHealth)
{
	CurrentHealth = CurrentHealth + NewHealth;
	if(CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	UpdateHealth();
}

void UPlayerStatsComponent::UpdateHealth()
{
	OnUpdateHealthBar.Broadcast(CurrentHealth / MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), CurrentHealth);
}

void UPlayerStatsComponent::DecreaseStamina(float StaminaCost)
{
}

void UPlayerStatsComponent::IncreaseMaxStamina(float NewMaxStamina)
{
}

void UPlayerStatsComponent::IncreaseStamina(float NewStamina)
{
}

void UPlayerStatsComponent::UpdateStamina()
{
}


// Called every frame
void UPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

