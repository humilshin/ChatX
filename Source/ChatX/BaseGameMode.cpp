// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseGameMode.h"
#include "BaseGameState.h"
#include "BasePlayerController.h"
#include "EngineUtils.h"
#include "BasePlayerState.h"

void ABaseGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(NewPlayer);
	if (IsValid(PlayerController) == true)
	{
		PlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		AllPlayerControllers.Add(PlayerController);

		ABasePlayerState* PS = PlayerController->GetPlayerState<ABasePlayerState>();
		if (IsValid(PS) == true)
		{
			PS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ABaseGameState* GameStateBase = GetGameState<ABaseGameState>();
		if (IsValid(GameStateBase) == true)
		{
			GameStateBase->MulticastRPCBroadcastLoginMessage(PS->PlayerNameString);
		}

		if (AllPlayerControllers.Num() == 1)
		{
			StartTurn();
		}
	}
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("[Server] Secret Number: %s"), *SecretNumberString);
}


FString ABaseGameMode::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ABaseGameMode::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			if (UniqueDigits.Contains(C) == true)
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ABaseGameMode::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ABaseGameMode::PrintChatMessageString(ABasePlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	int32 SeparatorIndex = InChatMessageString.Find(TEXT(": "));
	FString RawInput = InChatMessageString.Mid(SeparatorIndex + 2);

	bool bIsDigitsOnly = (RawInput.Len() > 0);
	for (TCHAR C : RawInput)
	{
		if (FChar::IsDigit(C) == false)
		{
			bIsDigitsOnly = false;
			break;
		}
	}

	if (bIsDigitsOnly == true)
	{
		if (RawInput.Len() != 3)
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("3자리 숫자를 입력해주세요."));
			return;
		}

		if (RawInput.Contains(TEXT("0")) == true)
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("0을 제외한 1~9 사이의 숫자를 입력해주세요."));
			return;
		}

		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : RawInput)
		{
			UniqueDigits.Add(C);
		}
		if (UniqueDigits.Num() != RawInput.Len())
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("중복되지 않은 숫자를 입력해주세요."));
			return;
		}

		FString JudgeResultString = JudgeResult(SecretNumberString, RawInput);
		IncreaseGuessCount(InChattingPlayerController);

		FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
		for (TActorIterator<ABasePlayerController> It(GetWorld()); It; ++It)
		{
			ABasePlayerController* PC = *It;
			if (IsValid(PC) == true)
			{
				PC->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}

		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		bHasActedThisTurn = true;
		GetWorldTimerManager().ClearTimer(TurnTimerHandle);

		bool bGameReset = JudgeGame(InChattingPlayerController, StrikeCount);
		if (bGameReset == false)
		{
			EndTurn();
		}
	}
	else
	{
		for (TActorIterator<ABasePlayerController> It(GetWorld()); It; ++It)
		{
			ABasePlayerController* PC = *It;
			if (IsValid(PC) == true)
			{
				PC->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ABaseGameMode::IncreaseGuessCount(ABasePlayerController* InChattingPlayerController)
{
	ABasePlayerState* PS = InChattingPlayerController->GetPlayerState<ABasePlayerState>();
	if (IsValid(PS) == true)
	{
		PS->CurrentGuessCount++;
	}
}

void ABaseGameMode::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("[Server] New Secret Number: %s"), *SecretNumberString);

	for (const auto& PlayerController : AllPlayerControllers)
	{
		ABasePlayerState* PS = PlayerController->GetPlayerState<ABasePlayerState>();
		if (IsValid(PS) == true)
		{
			PS->CurrentGuessCount = 0;
		}

		PlayerController->ClientRPCClearChatLog();
	}

	CurrentTurnPlayerIndex = 0;
	StartTurn();
}

bool ABaseGameMode::JudgeGame(ABasePlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ABasePlayerState* PS = InChattingPlayerController->GetPlayerState<ABasePlayerState>();
		if (IsValid(PS) == true)
		{
			FString WinMessage = PS->PlayerNameString + TEXT(" has won the game.");
			for (const auto& PlayerController : AllPlayerControllers)
			{
				PlayerController->NotificationText = FText::FromString(WinMessage);
			}
		}
		ResetGame();
		return true;
	}

	return false;
}

void ABaseGameMode::StartTurn()
{
	if (AllPlayerControllers.Num() == 0)
	{
		return;
	}

	bHasActedThisTurn = false;

	ABaseGameState* GS = GetGameState<ABaseGameState>();
	if (IsValid(GS) == true)
	{
		GS->RemainingTime = TurnDuration;

		ABasePlayerController* CurrentPC = AllPlayerControllers[CurrentTurnPlayerIndex];
		ABasePlayerState* PS = CurrentPC->GetPlayerState<ABasePlayerState>();
		if (IsValid(PS) == true)
		{
			GS->CurrentTurnPlayerName = PS->PlayerNameString;
		}
	}

	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ABaseGameMode::OnTurnTimerTick, 1.0f, true);
}

void ABaseGameMode::OnTurnTimerTick()
{
	ABaseGameState* GS = GetGameState<ABaseGameState>();
	if (IsValid(GS) == true)
	{
		GS->RemainingTime--;

		if (GS->RemainingTime <= 0)
		{
			GS->RemainingTime = 0;
			EndTurn();
		}
	}
}

void ABaseGameMode::EndTurn()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);

	if (bHasActedThisTurn == false)
	{
		ABasePlayerController* CurrentPC = AllPlayerControllers[CurrentTurnPlayerIndex];
		IncreaseGuessCount(CurrentPC);
		CurrentPC->ClientRPCPrintChatMessageString(TEXT("시간 초과! 기회가 1회 차감되었습니다."));
	}

	bool bIsDraw = true;
	for (const auto& PlayerController : AllPlayerControllers)
	{
		ABasePlayerState* PS = PlayerController->GetPlayerState<ABasePlayerState>();
		if (IsValid(PS) == true && PS->CurrentGuessCount < PS->MaxGuessCount)
		{
			bIsDraw = false;
			break;
		}
	}

	if (bIsDraw == true)
	{
		for (const auto& PlayerController : AllPlayerControllers)
		{
			PlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
		}
		ResetGame();
		return;
	}

	int32 NumPlayers = AllPlayerControllers.Num();
	for (int32 i = 1; i <= NumPlayers; ++i)
	{
		int32 NextIndex = (CurrentTurnPlayerIndex + i) % NumPlayers;
		ABasePlayerState* PS = AllPlayerControllers[NextIndex]->GetPlayerState<ABasePlayerState>();
		if (IsValid(PS) == true && PS->CurrentGuessCount < PS->MaxGuessCount)
		{
			CurrentTurnPlayerIndex = NextIndex;
			StartTurn();
			return;
		}
	}
}
