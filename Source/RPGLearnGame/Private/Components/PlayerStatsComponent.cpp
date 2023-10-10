#include "Components/PlayerStatsComponent.h"

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

void UPlayerStatsComponent::DecreaseHealth(const float Damage, bool& IsPlayerDead)
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

void UPlayerStatsComponent::IncreaseMaxHealth(const float NewMaxHealth)
{
	MaxHealth = MaxHealth + NewMaxHealth;
	UpdateHealth();
}

void UPlayerStatsComponent::IncreaseHealth(const float NewHealth)
{
	CurrentHealth = CurrentHealth + NewHealth;
	if(CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	UpdateHealth();
}

void UPlayerStatsComponent::UpdateHealth() const
{
	OnUpdateHealthBar.Broadcast(CurrentHealth / MaxHealth);
}

void UPlayerStatsComponent::DecreaseStamina(const float StaminaCost, bool& IsPlayerDead)
{
	if(CurrentStamina <= 0.f)
	{
		IsPlayerDead = true;
	}
	else
	{
		CurrentStamina = CurrentStamina - StaminaCost;
		UpdateStamina();
		IsPlayerDead = (CurrentStamina <= 0.f);
	}
}

void UPlayerStatsComponent::IncreaseMaxStamina(const float NewMaxStamina)
{
	MaxStamina = MaxStamina + NewMaxStamina;
	UpdateStamina();
}

void UPlayerStatsComponent::IncreaseStamina(const float NewStamina)
{
	CurrentStamina = CurrentStamina + NewStamina;
	if(CurrentStamina > MaxStamina)
	{
		CurrentStamina = MaxStamina;
	}
	UpdateStamina();
}

void UPlayerStatsComponent::UpdateStamina()
{
	OnUpdateStaminaBar.Broadcast(CurrentStamina / MaxStamina);
}


// Called every frame
void UPlayerStatsComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

