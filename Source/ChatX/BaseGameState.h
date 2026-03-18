// BaseGameState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseGameState.generated.h"

/**
 *
 */
UCLASS()
class CHATX_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime)
	int32 RemainingTime;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentTurnPlayerName)
	FString CurrentTurnPlayerName;

	UFUNCTION()
	void OnRep_RemainingTime();

	UFUNCTION()
	void OnRep_CurrentTurnPlayerName();
};
