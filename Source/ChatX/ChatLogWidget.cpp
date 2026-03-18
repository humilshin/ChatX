// ChatLogWidget.cpp

#include "ChatLogWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UChatLogWidget::AddChatMessage(const FString& InMessage)
{
	if (IsValid(ScrollBox_ChatLog) == true)
	{
		UTextBlock* NewTextBlock = NewObject<UTextBlock>(ScrollBox_ChatLog);
		NewTextBlock->SetText(FText::FromString(InMessage));
		ScrollBox_ChatLog->AddChild(NewTextBlock);
		ScrollBox_ChatLog->ScrollToEnd();
	}
}

void UChatLogWidget::ClearChatLog()
{
	if (IsValid(ScrollBox_ChatLog) == true)
	{
		ScrollBox_ChatLog->ClearChildren();
	}
}
