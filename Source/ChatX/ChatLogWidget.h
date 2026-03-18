// ChatLogWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatLogWidget.generated.h"

class UScrollBox;

UCLASS()
class CHATX_API UChatLogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddChatMessage(const FString& InMessage);
	void ClearChatLog();

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ChatLog;
};
