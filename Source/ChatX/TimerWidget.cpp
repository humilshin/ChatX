// TimerWidget.cpp

#include "TimerWidget.h"

#include "Components/TextBlock.h"

void UTimerWidget::SetRemainingTime(int32 InRemainingTime)
{
	if (IsValid(TextBlock_Timer) == true)
	{
		TextBlock_Timer->SetText(FText::FromString(FString::Printf(TEXT("%d"), InRemainingTime)));
	}
}
