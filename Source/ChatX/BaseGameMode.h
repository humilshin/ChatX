// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

class ABasePlayerController;

/**
 *
 */
UCLASS()
class CHATX_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(ABasePlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(ABasePlayerController* InChattingPlayerController);

	void ResetGame();

	bool JudgeGame(ABasePlayerController* InChattingPlayerController, int InStrikeCount);

	void StartTurn();
	void EndTurn();
	void OnTurnTimerTick();

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ABasePlayerController>> AllPlayerControllers;

	UPROPERTY(EditDefaultsOnly)
	int32 TurnDuration = 30;

	int32 CurrentTurnPlayerIndex = 0;
	bool bHasActedThisTurn = false;
	FTimerHandle TurnTimerHandle;
};
