// NotificationWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidget.generated.h"

class UTextBlock;

UCLASS()
class CHATX_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNotificationText(const FText& InText);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Notification;
};
