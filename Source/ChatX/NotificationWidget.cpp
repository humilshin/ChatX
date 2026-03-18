// NotificationWidget.cpp

#include "NotificationWidget.h"

#include "Components/TextBlock.h"

void UNotificationWidget::SetNotificationText(const FText& InText)
{
	if (IsValid(TextBlock_Notification) == true)
	{
		TextBlock_Notification->SetText(InText);
	}
}
