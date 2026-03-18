// TimerWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

class UTextBlock;

UCLASS()
class CHATX_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetRemainingTime(int32 InRemainingTime);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Timer;
};
