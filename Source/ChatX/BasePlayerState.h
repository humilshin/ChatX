// BasePlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CHATX_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABasePlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;
	
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
	FString GetPlayerInfoString();
};