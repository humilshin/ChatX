// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UChatInput;
class UNotificationWidget;
class UChatLogWidget;
class UTimerWidget;

UCLASS()
class CHATX_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCClearChatLog();

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateTimerDisplay(int32 InRemainingTime);
	void UpdateTurnInfo(const FString& InCurrentTurnPlayerName);
	void UpdateInputState();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNotificationWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UNotificationWidget> NotificationTextWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatLogWidget> ChatLogWidgetClass;

	UPROPERTY()
	TObjectPtr<UChatLogWidget> ChatLogWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTimerWidget> TimerWidgetClass;

	UPROPERTY()
	TObjectPtr<UTimerWidget> TimerWidgetInstance;

public:
	UPROPERTY(ReplicatedUsing = OnRep_NotificationText, BlueprintReadOnly)
	FText NotificationText;

	UFUNCTION()
	void OnRep_NotificationText();

	void HideNotificationWidget();

	FTimerHandle NotificationTimerHandle;
};
