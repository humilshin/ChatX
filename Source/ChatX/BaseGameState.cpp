// BaseGameState.cpp

#include "BaseGameState.h"

#include "Kismet/GameplayStatics.h"
#include "BasePlayerController.h"
#include "Net/UnrealNetwork.h"

void ABaseGameState::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ABasePlayerController* BasePC = Cast<ABasePlayerController>(PC);
			if (IsValid(BasePC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				BasePC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, CurrentTurnPlayerName);
}

void ABaseGameState::OnRep_RemainingTime()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC) == true)
	{
		ABasePlayerController* BasePC = Cast<ABasePlayerController>(PC);
		if (IsValid(BasePC) == true)
		{
			BasePC->UpdateTimerDisplay(RemainingTime);
		}
	}
}

void ABaseGameState::OnRep_CurrentTurnPlayerName()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC) == true)
	{
		ABasePlayerController* BasePC = Cast<ABasePlayerController>(PC);
		if (IsValid(BasePC) == true)
		{
			BasePC->UpdateTurnInfo(CurrentTurnPlayerName);
		}
	}
}
