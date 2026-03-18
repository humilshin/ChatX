#include "BasePlayerController.h"

#include "ChatInput.h"
#include "NotificationWidget.h"
#include "ChatLogWidget.h"
#include "TimerWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BaseGameMode.h"
#include "BaseGameState.h"
#include "BasePlayerState.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

ABasePlayerController::ABasePlayerController()
{
	bReplicates = true;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UNotificationWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
			NotificationTextWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (IsValid(ChatLogWidgetClass) == true)
	{
		ChatLogWidgetInstance = CreateWidget<UChatLogWidget>(this, ChatLogWidgetClass);
		if (IsValid(ChatLogWidgetInstance) == true)
		{
			ChatLogWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(TimerWidgetClass) == true)
	{
		TimerWidgetInstance = CreateWidget<UTimerWidget>(this, TimerWidgetClass);
		if (IsValid(TimerWidgetInstance) == true)
		{
			TimerWidgetInstance->AddToViewport();
		}
	}
}

void ABasePlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
		if (IsValid(PS) == true)
		{
			FString CombinedMessageString = PS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ABasePlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	if (IsValid(ChatLogWidgetInstance) == true)
	{
		ChatLogWidgetInstance->AddChatMessage(InChatMessageString);
	}
}

void ABasePlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ABasePlayerController::ClientRPCClearChatLog_Implementation()
{
	if (IsValid(ChatLogWidgetInstance) == true)
	{
		ChatLogWidgetInstance->ClearChatLog();
	}
}

void ABasePlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ABaseGameMode* BaseGM = Cast<ABaseGameMode>(GM);
		if (IsValid(BaseGM) == true)
		{
			BaseGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ABasePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ABasePlayerController::OnRep_NotificationText()
{
	if (IsValid(NotificationTextWidgetInstance) == true)
	{
		NotificationTextWidgetInstance->SetNotificationText(NotificationText);
		NotificationTextWidgetInstance->SetVisibility(ESlateVisibility::Visible);

		GetWorldTimerManager().ClearTimer(NotificationTimerHandle);
		GetWorldTimerManager().SetTimer(NotificationTimerHandle, this, &ABasePlayerController::HideNotificationWidget, 5.0f, false);
	}
}

void ABasePlayerController::HideNotificationWidget()
{
	if (IsValid(NotificationTextWidgetInstance) == true)
	{
		NotificationTextWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABasePlayerController::UpdateTimerDisplay(int32 InRemainingTime)
{
	if (IsValid(TimerWidgetInstance) == true)
	{
		TimerWidgetInstance->SetRemainingTime(InRemainingTime);
	}

	UpdateInputState();
}

void ABasePlayerController::UpdateTurnInfo(const FString& InCurrentTurnPlayerName)
{
	FString TurnMessage = InCurrentTurnPlayerName + TEXT("의 차례입니다.");
	PrintChatMessageString(TurnMessage);

	UpdateInputState();
}

void ABasePlayerController::UpdateInputState()
{
	if (IsValid(ChatInputWidgetInstance) == false)
	{
		return;
	}

	bool bShouldEnable = false;

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	ABaseGameState* GS = GetWorld()->GetGameState<ABaseGameState>();

	if (IsValid(PS) == true && IsValid(GS) == true)
	{
		bool bIsMyTurn = (PS->PlayerNameString == GS->CurrentTurnPlayerName);
		bool bHasTime = (GS->RemainingTime > 0);
		bShouldEnable = bIsMyTurn && bHasTime;
	}

	ChatInputWidgetInstance->SetInputEnabled(bShouldEnable);
}
